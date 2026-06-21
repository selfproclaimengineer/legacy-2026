%x_polinom = [150, 100, 50, 0];
%y_polinom = [0, 50, 100, 150];
%z_polinom = [0, 132, 132, 0];
x1 = 97;
x3 = 163;
Y1 = x3;
y3 = x1;
zp = 130;
z1 = 97; %Zpuncak = 0.25*z1 + 0.75 * z3
z3 = (zp - (0.25 * z1))/0.75;
x = zeros(1, 50);
y = zeros(1, 50);
z = zeros(1, 50);

x_polinom = [ x1, x1, x3, x3 ];
y_polinom = [ Y1, Y1, y3, y3 ];
z_polinom = [ z1, z3, z3, z1 ];
iterasi = 0.01;
i =0;
for t = 0:iterasi:1
    i = i+1;
    a = (1 - t) * (1 - t) * (1 - t);
    b = 3 * t * (1 - t) * (1 - t);
    c = 3 * t * t * (1 - t);
    d = t * t * t;
    x(i,:) = ((((a*x_polinom(1))+b*x_polinom(2))+c*x_polinom(3))+d*x_polinom(4));
    y(i,:) = ((((a*y_polinom(1))+b*y_polinom(2))+c*y_polinom(3))+d*y_polinom(4));
    z(i,:)=  ((((a*z_polinom(1))+b*z_polinom(2))+c*z_polinom(3))+d*z_polinom(4));
end

x_polinom = [ x1, x1, x3, x3 ];
y_polinom = [ y1, y1, y3, y3 ];
z_polinom = [ z1, z1, z3, z3 ];
iterasi = 0.01;
for t = 1:iterasi:0
    i = i+1;
    a = (1 - t) * (1 - t) * (1 - t);
    b = 3 * t * (1 - t) * (1 - t);
    c = 3 * t * t * (1 - t);
    d = t * t * t;
    x(i,:) = ((((a*x_polinom(1))+b*x_polinom(2))+c*x_polinom(3))+d*x_polinom(4));
    y(i,:) = ((((a*y_polinom(1))+b*y_polinom(2))+c*y_polinom(3))+d*y_polinom(4));
    z(i,:)=  ((((a*z_polinom(1))+b*z_polinom(2))+c*z_polinom(3))+d*z_polinom(4));
end

plot3(x,y,z)
grid
xlabel('x')
ylabel('y')
zlabel('z')