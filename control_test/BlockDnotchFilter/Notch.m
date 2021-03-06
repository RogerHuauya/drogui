wo = 5/(1000/2);  
bw = wo/10;
Fs=1000;
[b,a] = iirnotch(wo,bw);
roll_wf = filter(b,a,roll_w);
hold on;
figure(1);
plot(roll_w,'b');
plot(roll_wf,'g');
figure(2)
hold on;
plotFreq(roll_w,Fs);
plotFreq(roll_wf,Fs);