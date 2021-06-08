close all
sys = iddata([VarName3, VarName6], [VarName4, VarName7, VarName9], 0.02);
sys.InputName  = {'R';'P';'H'};
sys.OutputName = {'roll';'pitch'};
%%
plot(sys(:,1,1))
mi = impulseest(sys,10);
clf, step(mi)
showConfidence(impulseplot(mi),3)
%%
figure
mp = ssest(sys(1:85));
g = stepplot(mi,'b',mp,'r',2);
showConfidence(g)
%%

figure 
compare(sys(50:80),mp)

%%
m1 = tfest(sys(1:85,'roll','R'),2,1); % TF model with 2 poles 1 zero
m2 = tfest(sys(1:85,'pitch','P'),3,0); % Simple TF model with 1 pole.
compare(sys(1:85),mp, m1, m2);

%%
sys2 = iddata([roll, pitch], [rollu, pitchu, h], 0.02);
sys2.InputName  = {'R';'P';'H'};
sys2.OutputName = {'roll';'pitch'};

compare(sys2(1:76),mp, m1, m2);



