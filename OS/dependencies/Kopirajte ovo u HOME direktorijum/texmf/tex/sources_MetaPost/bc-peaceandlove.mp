prologues:=1;
outputtemplate:="%j.mps";
beginfig(1);
pickup pencircle scaled(3bp);
draw fullcircle scaled(38);
draw (0,-19)--(0,19);
draw (0,0)--((0,-19) rotated(45));
draw (0,0)--((0,-19) rotated(-45));
r:=17pt/(xpart(urcorner(currentpicture))-xpart(ulcorner(currentpicture)));
picture pic;
pic:=currentpicture;
currentpicture:=nullpicture;
drawoptions();
draw pic scaled(r);
z0=.5*urcorner(currentpicture)+.5*ulcorner(currentpicture);
z1=.5*lrcorner(currentpicture)+.5*llcorner(currentpicture);
draw (z0+(10.5,2));
draw (z1-(10.5,2));
clip currentpicture to ((z0-(8.5pt,0))--(z0+(8.5pt,0))--(z1+(8.5pt,0))--(z1-(8.5pt,0))--cycle);
endfig;
end;
