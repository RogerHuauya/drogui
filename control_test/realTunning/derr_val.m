function [der_err_roll,der_err_pitch] = derr_val(roll_data,pitch_data,coeff,f)
% coeff = f*[-12/60,75/60,-200/60,300/60,-300/60,137/60];
roll_data = -1*roll_data;
pitch_data = -1*pitch_data;
coeff = f*coeff;
dim_c = size(coeff);
dim_d = size(pitch_data);
der_err_roll = [];
der_err_pitch = [];
    for i = dim_c(2)+1:dim_d(1)
        aux_r = roll_data(i-dim_c(2):i-1)'*coeff';
        aux_p = pitch_data(i-dim_c(2):i-1)'*coeff' ;
        der_err_roll = [der_err_roll,aux_r];
        der_err_pitch = [der_err_pitch,aux_p];    
    end
end

