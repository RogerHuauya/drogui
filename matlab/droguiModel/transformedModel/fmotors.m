function [FM] = fmotors(u)
  F = 9.81/1000*( -0.0026*u.*u.*u + 0.4892*u.*u - 4.2855*u + 0.8182);
  M = 0.05*F;
  FM = [F;M];
end