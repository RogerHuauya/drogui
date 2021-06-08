close all
sys = iddata([VarName3, VarName6], [VarName4, VarName7, VarName9], 0.02);
sys.InputName  = {'R';'P';'H'};
sys.OutputName = {'roll';'pitch'};
%%
plot(sys(:,1,1))
mi = impulseest(sys,50);
clf, step(mi)
showConfidence(impulseplot(mi),1)
%%
figure
mp = ssest(sys(1:500),1);
g = stepplot(mi,'b',mp,'r',2);
showConfidence(g)
%%

figure 
compare(sys,mp)

%%
m1 = tfest(sys(1:200,'roll','R'),1,1); % TF model with 2 poles 1 zero
m2 = tfest(sys(1:200,'pitch','P'),1,1); % Simple TF model with 1 pole.
compare(sys,mp, m1, m2);

%%
sys2 = iddata([roll, pitch], [rollu, pitchu, h], 0.02);
sys2.InputName  = {'R';'P';'H'};
sys2.OutputName = {'roll';'pitch'};

compare(sys2(1:76),mp, m1, m2);



