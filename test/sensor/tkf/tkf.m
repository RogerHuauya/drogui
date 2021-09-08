classdef tkf
    properties
        q = zeros(4,1)
        P
        Q0
        V1
        V2
        R1
        R2
        Ts
        G
        I = diag(ones(1,4))
    end
    methods
        function obj = tkf(Ts, G, P, Q, R1, R2)
            obj.Ts = Ts;
            obj.G = G;
            obj.Q0 = Q;
            obj.P = P;
            obj.q(1,1) = 1;
            obj.V1 = diag(ones(1,3));
            obj.V2 = diag(ones(1,3));
            obj.R1 =  R1;
            obj.R2 =  R2;
        end
        function obj = updateStage1(obj, wx, wy, wz, ax, ay, az)
           
            matrixW = [ 0,  -wx, -wy, -wz; 
                       wx,    0,  wz, -wy; 
                       wy,  -wz,   0,  wx;
                       wz,   wy, -wx,   0];
            
            A = obj.I + 0.5*obj.Ts*matrixW;
            obj.q = A*obj.q;
            obj.P =  A*obj.P*(A') + obj.Q0;
            
            q0 = obj.q(1,1);
            q1 = obj.q(2,1);
            q2 = obj.q(3,1);
            q3 = obj.q(4,1);
            z1 = [ax,ay,az]';
            
            H1 = [-2*q2,  2*q3, -2*q0, 2*q1;
                   2*q1,  2*q0,  2*q3, 2*q2;
                   2*q0, -2*q1, -2*q2, 2*q3];
             
            h1 = obj.G*[ 2*q1*q3 - 2*q0*q2; 
                        2*q0*q1 + 2*q2*q3;
                        (q0^2) - (q1^2) - (q2^2) + (q3^2) ];
            
            %disp(obj.P)
            %disp(H1')
            %disp(obj.P*H1')
            K1 = obj.P*(H1')*( (H1*obj.P*(H1') + obj.V1*obj.R1*(obj.V1')  )^(-1) ); 
            
            q_estimate = K1*(z1-h1);
            q_estimate(4,1) = 0;
            
            obj.q = obj.q + q_estimate;
%             obj.Paux = obj.P;
            %disp(obj.I - K1*H1 )
            obj.P = ( obj.I - K1*H1 )*obj.P;
            %disp(obj.P)
            
        end
        function obj = updateStage2(obj, mx, my, mz)
            
            
            q0 = obj.q(1,1);
            q1 = obj.q(2,1);
            q2 = obj.q(3,1);
            q3 = obj.q(4,1);
            z2 = [mx,my,mz]';
            
            H2 = [ 2*q3,  2*q2,  2*q1,  2*q0;
                   2*q0, -2*q1, -2*q2, -2*q3;
                  -2*q1, -2*q0,  2*q3,  2*q2];
              
            h2 = [ 2*q1*q2 + 2*q3*q0;
                    q0^2 - q1^2 - q2^2 - q3^2;
                    2*q2*q3 - 2*q0*q1];
            disp(H2*obj.P*(H2'))
%             obj.K2 = obj.Paux*(H2')*( ( H2*obj.Paux*(H2') + obj.V2*obj.R2*(obj.V2') )^(-1) );
            K2 = obj.P*(H2')*( ( H2*obj.P*(H2') + obj.V2*obj.R2*(obj.V2') )^(-1) );
            q_estimate = K2*(z2-h2);
            q_estimate(2:3,1) = 0;
            obj.q = obj.q + q_estimate;
            obj.P = ( obj.I - K2*H2 )*obj.P;
        end
        
        %function [roll, pitch , yaw] = getTkfEuler(obj)
        %    eulerAngles = quat2eul(obj.q','XYZ');
        %    roll = eulerAngles(1);
        %    pitch = eulerAngles(2);
        %    yaw = eulerAngles(3);
        %end
        
        function [roll, pitch , yaw] = getTkfEuler(obj)
            
            q0 = obj.q(1,1);
            q1 = obj.q(2,1);
            q2 = obj.q(3,1);
            q3 = obj.q(4,1);
            
			% roll (x-axis rotation)
			sinr_cosp = 2 * ((q0) * (q1) + (q2) * (q3));
			cosr_cosp = 1 - 2 * ((q1) * (q1) + (q2) * (q2));
			roll = atan2(sinr_cosp, cosr_cosp);

			% pitch (y-axis rotation)
			sinp = 2 * ((q0) * (q2) - (q3) * (q1));
			if (abs(sinp) >= 1)
				pitch = (pi / 2)*sign(sinp); % use 90 degrees if out of range
			else
				pitch = asin(sinp);

				% yaw (z-axis rotation)
				siny_cosp = 2 * ((q0) * (q3) + (q1) * (q2));
				cosy_cosp = 1 - 2 * ((q2) * (q2) + (q3) * (q3));
				yaw = atan2(siny_cosp, cosy_cosp);
			end
        end
        
    end
end