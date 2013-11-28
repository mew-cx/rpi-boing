$fn=45;
phi=(1+sqrt(5))/2;
r1=2.2;

union()
{
color([0.2,0.2,0.2,1]) sphere(r=3.7);

color([0.74,0.06,0.28,1])
{
translate([ 0, 1, phi]) sphere(r1);
translate([ 0, 1,-phi]) sphere(r1);
translate([ 0,-1, phi]) sphere(r1);
translate([ 0,-1,-phi]) sphere(r1);

translate([ 1, phi, 0]) sphere(r1);
translate([ 1,-phi, 0]) sphere(r1);
translate([-1, phi, 0]) sphere(r1);
translate([-1,-phi, 0]) sphere(r1);

translate([ phi, 0, 1]) sphere(r1);
translate([-phi, 0, 1]) sphere(r1);
translate([ phi, 0,-1]) sphere(r1);
translate([-phi, 0,-1]) sphere(r1);
}
}