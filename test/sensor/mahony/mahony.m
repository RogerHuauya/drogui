classdef mahony
	properties
		sampleFreq
		twoKp
		twoKi
		q0 = 1
		q1 = 0
		q2 = 0
		q3 = 0
		integralFBx = 0
		integralFBy = 0
		integralFBz = 0
	end
	methods
        function obj = mahony(kp, ki, sampleFreq)
            obj.twoKp = 2*kp;
            obj.twoKi = 2*ki;
            obj.sampleFreq = sampleFreq;
        end
		function obj = mahonyUpdate(obj, gx,gy,gz,ax,ay,az,mx,my,mz)

			halfex = 0;
			halfey = 0;
			halfez = 0;
			useAcc = true;
			G = 9.81;
			if( (mx == 0.0 && my == 0.0 && mz == 0.0 ) || isnan(mx) || isnan(my) || isnan(mz))
				obj = mahonyUpdateIMU(obj, gx, gy, gz, ax, ay, az);
                disp('sin mag')
				return
			end
			accMod = sqrt(ax*ax + ay*ay + az*az);
			if( ((ax == 0.0) && (ay == 0.0) && (az == 0.0)) || accMod < G*0.8 || accMod > 1.2*G)
				
                useAcc = false;
			end

			q0q0 = (obj.q0) * (obj.q0);
			q0q1 = (obj.q0) * (obj.q1);
			q0q2 = (obj.q0) * (obj.q2);
			q0q3 = (obj.q0) * (obj.q3);
			q1q1 = (obj.q1) * (obj.q1);
			q1q2 = (obj.q1) * (obj.q2);
			q1q3 = (obj.q1) * (obj.q3);
			q2q2 = (obj.q2) * (obj.q2);
			q2q3 = (obj.q2) * (obj.q3);
			q3q3 = (obj.q3) * (obj.q3);

			recipNorm = 1/sqrt(mx * mx + my * my + mz * mz);
			mx = mx*recipNorm;
			my = my*recipNorm;
			mz = mz*recipNorm;

			% Reference direction of Earth's magnetic field
			hx = 2.0 * (mx * (0.5 - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
			hy = 2.0 * (mx * (q1q2 + q0q3) + my * (0.5 - q1q1 - q3q3) + mz * (q2q3 - q0q1));
			bx = sqrt(hx * hx + hy * hy);
			bz = 2.0 * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5 - q1q1 - q2q2));

			% Estimated direction of gravity and magnetic field
			halfvx = q1q3 - q0q2;
			halfvy = q0q1 + q2q3;
			halfvz = q0q0 - 0.5 + q3q3;
			halfwx = bx * (0.5 - q2q2 - q3q3) + bz * (q1q3 - q0q2);
			halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
			halfwz = bx * (q0q2 + q1q3) + bz * (0.5 - q1q1 - q2q2);

			% Error is sum of cross product between estimated direction and measured direction of field vectors
			halfex = halfex + (my * halfwz - mz * halfwy);
			halfey = halfey + (mz * halfwx - mx * halfwz);
			halfez = halfez + (mx * halfwy - my * halfwx);

			if(useAcc == true)
				% Normalise accelerometer measurement
                %disp("useAcc")
				recipNorm = 1/sqrt(ax * ax + ay * ay + az * az);
				ax = ax * recipNorm;
				ay = ay * recipNorm;
				az = az * recipNorm;
            
				% Error is sum of cross product between estimated direction and measured direction of field vectors
				halfex = halfex + (ay * halfvz - az * halfvy);
				halfey = halfey + (az * halfvx - ax * halfvz);
				halfez = halfez + (ax * halfvy - ay * halfvx);
            else
                halfex = 0;
				halfey = 0;
				halfez = 0;
            end
           


			% Compute and apply integral feedback if enabled
			if((obj.twoKi) > 0.0)
				obj.integralFBx = (obj.integralFBx) + (obj.twoKi) * halfex * (1.0 / (obj.sampleFreq));	% integral error scaled by Ki
				obj.integralFBy = (obj.integralFBy) + (obj.twoKi) * halfey * (1.0 / (obj.sampleFreq));
				obj.integralFBz = (obj.integralFBz) + (obj.twoKi) * halfez * (1.0 / (obj.sampleFreq));
				gx = gx + (obj.integralFBx);	% apply integral feedback
				gy = gy + (obj.integralFBy);
				gz = gz + (obj.integralFBz);

			else
				obj.integralFBx = 0.0;	% prevent integral windup
				obj.integralFBy = 0.0;
				obj.integralFBz = 0.0;

			end
			% Apply proportional feedback
			gx = gx + (obj.twoKp) * halfex;
			gy = gy + (obj.twoKp) * halfey;
			gz = gz + (obj.twoKp) * halfez;

			% Integrate rate of change of quaternion
			gx = gx * (0.5 * (1.0 / (obj.sampleFreq)));		% pre-multiply common actors
			gy = gy * (0.5 * (1.0 / (obj.sampleFreq)));
			gz = gz * (0.5 * (1.0 / (obj.sampleFreq)));
			qa = (obj.q0);
			qb = (obj.q1);
			qc = (obj.q2);

			obj.q0 = obj.q0 + (-qb * gx - qc * gy - obj.q3 * gz);
			obj.q1 = obj.q1 + (qa * gx + qc * gz - obj.q3 * gy);
			obj.q2 = obj.q2 + (qa * gy - qb * gz + obj.q3 * gx);
			obj.q3 = obj.q3 + (qa * gz + qb * gy - qc * gx);

			% Normalise quaternion
			recipNorm = 1/sqrt((obj.q0) * (obj.q0) + (obj.q1) * (obj.q1) + (obj.q2) * (obj.q2) + (obj.q3) * (obj.q3));
			obj.q0 = obj.q0 * recipNorm;
			obj.q1 = obj.q1 * recipNorm;
			obj.q2 = obj.q2 * recipNorm;
			obj.q3 = obj.q3 * recipNorm;
		end

		function obj = mahonyUpdateIMU(obj, gx, gy, gz, ax, ay, az)
			% Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
			if(~((ax == 0.0) && (ay == 0.0) && (az == 0.0)))

				% Normalise accelerometer measurement
				recipNorm = 1/sqrt(ax * ax + ay * ay + az * az);
				ax = ax * recipNorm;
				ay = ay * recipNorm;
				az = az * recipNorm;

				% Estimated direction of gravity and vector perpendicular to magnetic flux
				halfvx = (obj.q1) * (obj.q3) - (obj.q0) * (obj.q2);
				halfvy = (obj.q0) * (obj.q1) + (obj.q2) * (obj.q3);
				halfvz = (obj.q0) * (obj.q0) - 0.5 + (obj.q3) * (obj.q3);

				% Error is sum of cross product between estimated and measured direction of gravity
				halfex = (ay * halfvz - az * halfvy);
				halfey = (az * halfvx - ax * halfvz);
				halfez = (ax * halfvy - ay * halfvx);

				% Compute and apply integral feedback if enabled
				if((obj.twoKi) > 0.0)
					obj.integralFBx = obj.integralFBx + (obj.twoKi) * halfex * (1.0 / (obj.sampleFreq));	% integral error scaled by Ki
					obj.integralFBy = obj.integralFBy + (obj.twoKi) * halfey * (1.0 / (obj.sampleFreq));
					obj.integralFBz = obj.integralFBz + (obj.twoKi) * halfez * (1.0 / (obj.sampleFreq));
					gx = gx + (obj.integralFBx);	% apply integral feedback
					gy = gy + (obj.integralFBy);
					gz = gz + (obj.integralFBz);
				else
					obj.integralFBx = 0.0;	% prevent integral windup
					obj.integralFBy = 0.0;
					obj.integralFBz = 0.0;
				end

				% Apply proportional feedback
				gx = gx + (obj.twoKp) * halfex;
				gy = gy + (obj.twoKp) * halfey;
				gz = gz + (obj.twoKp) * halfez;
			end

			% Integrate rate of change of quaternion
			gx = gx * (0.5 * (1.0 / (obj.sampleFreq)));		% pre-multiply common factors
			gy = gy * (0.5 * (1.0 / (obj.sampleFreq)));
			gz = gz * (0.5 * (1.0 / (obj.sampleFreq)));
			qa = (obj.q0);
			qb = (obj.q1);
			qc = (obj.q2);
			obj.q0 = (obj.q0) + (-qb * gx - qc * gy - (obj.q3) * gz);
			obj.q1 = (obj.q1) + (qa * gx + qc * gz - (obj.q3) * gy);
			obj.q2 = (obj.q2) + (qa * gy - qb * gz + (obj.q3) * gx);
			obj.q3 = (obj.q3) + (qa * gz + qb * gy - qc * gx);

			% Normalise quaternion
			recipNorm = 1/sqrt((obj.q0) * (obj.q0) + (obj.q1) * (obj.q1) + (obj.q2) * (obj.q2) + (obj.q3) * (obj.q3));
			obj.q0 = (obj.q0) * recipNorm;
			obj.q1 = (obj.q1) * recipNorm;
			obj.q2 = (obj.q2) * recipNorm;
			obj.q3 = (obj.q3) * recipNorm;
		end
		function [roll, pitch , yaw] = getMahonyEuler(obj)

			% roll (x-axis rotation)
			sinr_cosp = 2 * ((obj.q0) * (obj.q1) + (obj.q2) * (obj.q3));
			cosr_cosp = 1 - 2 * ((obj.q1) * (obj.q1) + (obj.q2) * (obj.q2));
			roll = atan2(sinr_cosp, cosr_cosp);

			% pitch (y-axis rotation)
			sinp = 2 * ((obj.q0) * (obj.q2) - (obj.q3) * (obj.q1));
			if (abs(sinp) >= 1)
				pitch = (pi / 2)*sign(sinp); % use 90 degrees if out of range
			else
				pitch = asin(sinp);

				% yaw (z-axis rotation)
				siny_cosp = 2 * ((obj.q0) * (obj.q3) + (obj.q1) * (obj.q2));
				cosy_cosp = 1 - 2 * ((obj.q2) * (obj.q2) + (obj.q3) * (obj.q3));
				yaw = atan2(siny_cosp, cosy_cosp);
			end
        end
	end
end
