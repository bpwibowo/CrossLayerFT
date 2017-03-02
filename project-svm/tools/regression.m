%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Q-AVF regression analysis
%%
% Author:        Bagus Wibowo
% First created: 3/2/2016

function [B,bint,r,rint,stats] = regression(y,X)

X = [ones(size(X,1),1) X];
[B,bint,r,rint,stats] = regress(y,X);
