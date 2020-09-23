% Script file for running Blasius_Panel over an angle of attack sweep
clear variables
close all
clc

N = 150;                 % Number of panels
foil = '0012';          % Airfoil four digit NACA number

N = N - mod(N,2);       % Ensures that the number of panels is even
c = 1;                  % Chord length [m]

% Generate airfoil
[X,Y] = generateNACA4(foil,N);
X = flip(X);
Y = flip(Y);
Y(1) = 0;
Y(end) = 0;
Y = Y*c;
X = X*c;

rho = 1.225;                    % Freestream Density [kg/m^3]
mu = 1.802e-5;                  % Dynamic Viscosity [kg/m/s]
Re = rho*max(X)/mu;             % Reynolds Number based on chord (per unit velocity)
% Sets Vinf to ensure Re=3,000,000
Vinf = 3000000/Re;              % Freestream velocity [m/s]
Re = rho*Vinf*max(X)/mu;        % Reynolds Number based on chord

alphas = (-16:0.5:16);          % Angle of attack (AoA) [deg]
Cm = zeros(length(alphas),1);   % Moment Coefficient
Cd = Cm;                        % Drag Coefficient
Cl = Cm;                        % Lift Coefficient
Cli = Cm;                       % Inviscid Lift Coefficient

for i = 1:length(alphas)
    
    % Store the current AoA
    alpha = deg2rad(alphas(i));
    
    % Run an inviscid panel method at this AoA
    [Cp,~,X_mid,Y_mid,norms,tans] = inviscidPanel(X,Y,Vinf,alpha);
    
    % Get horizontal and vertical lengths of all panels
    dx = X(2:end) - X(1:end-1);
    dy = Y(2:end) - Y(1:end-1);
    
    % Get the slope of every panel
    dy_dx = zeros(length(X)-1,1);
    for j = 1:length(X)-1
        if j <= X/2
            dy_dx(j) = -(Y(j+1)-Y(j))/(X(j+1)-X(j));
        else
            dy_dx(j) = (Y(j+1)-Y(j))/(X(j+1)-X(j));
        end
    end
    
    % Calculate inviscid lift coefficient at this AoA
    Ca = 0;         % Normal coefficient
    Cn = 0;         % Axial coefficient
    for j = 1:length(Cp)/2
        Ca = Ca - (Cp(N-j+1)*dy_dx(N-j+1) - Cp(j)*dy_dx(j))*dx(j);
        Cn = Cn - (Cp(j) - Cp(N-j+1))*dx(j);
    end
    Cn = Cn/c;
    Ca = Ca/c;
    
    Cdi(i) = Ca*cos(alpha) + Cn*sin(alpha);     % Inviscid drag coefficient
    Cli(i) = Cn*cos(alpha) - Ca*sin(alpha);     % Inviscid lift coefficient
    
    % Run a viscous panel method at this AoA
    [Cp,Cf] = Blasius_Panel(X,Y,Vinf,alpha,mu,rho,20,false,false);
    
    % Calculate viscous lift coefficient at this AoA
    Ca = 0;         % Normal coefficient
    Cn = 0;         % Axial coefficient
    for j = 1:length(Cp)/2
        Ca = Ca - (Cp(N-j+1)*dy_dx(N-j+1) - Cp(j)*dy_dx(j))*dx(j) + (Cf(N-j+1) + Cf(j))*dx(j);
        Cn = Cn - (Cp(j) - Cp(N-j+1))*dx(j) + (Cf(N-j+1)*dy_dx(N-j+1) + Cf(j)*dy_dx(j))*dx(j);
    end
    Cn = Cn/c;
    Ca = Ca/c;
        
    Cm(i) = Cp'*(dx'.*X_mid + dy'.*Y_mid);      % Viscous moment coefficient
    Cd(i) = Ca*cos(alpha) + Cn*sin(alpha);      % Viscous drag coefficient
    Cl(i) = Cn*cos(alpha) - Ca*sin(alpha);      % Viscous lift coefficient
end

fprintf("Reynolds Number: %0.0f\n", Re)

load('0012_emp_data_2.mat')         % Empirical data
load('0012_aft_re1mil_data.mat')    % Airfoil tools

% Plot lift curves
figure()
plot(alphas,Cl,'linewidth',2)   % Viscous
hold on
plot(alphas,Cli,'--','linewidth',3)  % Inviscid
plot(alpha_0012_emp_2,cl_0012_emp_2,'linewidth',2)  % Empirical
plot(alpha_0012_airfoiltools,cl_0012_airfoiltools,'linewidth',2)    % Airfoiltools
plot([-15 15],[-15*pi/180*2*pi 15*pi/180*2*pi],'linewidth',2)   % Thin-airfoil
hold off
title('Lift Curve (Re=3,000,000)','interpreter','latex','fontsize',20)
xlabel('$\alpha$ (deg)','interpreter','latex','fontsize',18)
ylabel('$C_l$','interpreter','latex','fontsize',18)
legend('Viscous','Inviscid','Empirical','AirfoilTools','Thin-Airfoil','interpreter','latex','location','southeast','fontsize',16)
set(gcf, 'PaperPositionMode', 'manual')
set(gcf, 'Color', [1 1 1])
set(gca, 'Color', [1 1 1])
set(gcf, 'PaperUnits', 'centimeters')
set(gcf, 'PaperSize', [15 15])
set(gcf, 'Units', 'centimeters' )
set(gcf, 'Position', [0 0 15 15])
set(gcf, 'PaperPosition', [0 0 15 15])
grid on



% Get the error between the viscous and empirical
error = zeros(length(cl_0012_emp_2),1);
for i = 1:length(cl_0012_emp_2)
    v_value = interp1(alphas,Cl,alpha_0012_emp_2(i));
    error(i) = abs(v_value-cl_0012_emp_2(i));
end

% Plot viscous-empirical error
figure()
plot(alpha_0012_emp_2,error)
title("Viscous Error from Empirical")
xlabel("\alpha (deg)")
ylabel("\Delta C_l")

