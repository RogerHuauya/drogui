dim = length(accel(:,1));
Fs = 1000;
samples = 64;
L = samples;
f = Fs*(0:(L/2))/L;
arr_ans = [];
y_ant = [0 0];
x_ant = [0 0 0];
y_ant_dn = [0 0];
x_ant_dn = [0 0 0];
a = 1;
zeta = 1;
aux = accel(:,1);
aux_dn = zeros(1,length(accel(:,1)));
a_dn = 1;
zeta_dn = 5;
count = 1;
fc_ant = 0;
arr_fc = [];
arr_block =[];
for i=samples:dim
    y_act = aux(i-samples+1:i);
    if( mod(i,64) == 0 ) 
        block = 1;
    else
        block = 0;
        
    end
    [y,y_ant,x_ant,f_n] = dynamicnotchM(y_act,Fs,zeta,a,x_ant,y_ant,samples,block, fc_ant);
    arr_fc = [arr_fc,f_n];
    fc_ant = f_n;
    aux(i) = y;
end
