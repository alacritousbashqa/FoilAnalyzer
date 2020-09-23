% Script file for running Thwaites_cylinder function on a geometry
clear variables
close all
clc

N = 400;                 % Number of panels
foil = 'circle';          % Airfoil four digit NACA number

N = N - mod(N,2);       % Ensures that the number of panels is even

% Generate airfoil
[X,Y] = generateNACA4(foil,N);
X = flip(X);
Y = flip(Y);
Y(1) = 0;
Y(end) = 0;

c = 1;
X = c*X;
Y = c*Y;

rho = 1.225;            % Freestream Density [kg/m^3]
mu = 1.802e-5;          % Dynamic Viscosity [kg/m/s]
Vinf = .7355*1;         % Freestream velocity [m/s]
Re = rho*Vinf*c/mu;     % Chord Reynolds Number
alpha = deg2rad(0);     % Angle of attack [rad]

% Run a viscous panel method at this AoA
A = Thwaites_cylinder(X,Y,Vinf,alpha,mu,rho,1,false,false);

% Plot m and l
figure()
plot(A(:,7),A(:,4))
title("m")
xlabel("x (Surface Length)")
hold on
plot(A(:,7),A(:,8))
hold off
xlim([0 3*pi/4])
ylim([-.25 .4])
legend('m (code)','l (code)','location','southeast')
grid on

% Output the simulation parameters
fprintf("Reynolds Number: %0.0f\n", Re)
fprintf("Alpha: %0.2f deg\n", rad2deg(alpha))


