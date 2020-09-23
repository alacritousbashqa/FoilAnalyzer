% Script file for running Blasius_Panel to obtain the aerodynamic force over a specified region of the geometry
clear variables
close all
clc

%% Setup parameters
N = 150;                 % Number of panels
foil = '0035';          % Airfoil four digit NACA number
alpha = deg2rad(0);     % Angle of attack (AoA) [deg]
showPlots = true;      % Show plots at the end of each run

%% Run for different chords [in]
N = N - mod(N,2);       % Ensures that the number of panels is even
for chords = 12*3.28
    c_inch = chords;
    c = c_inch/12/3.28;          % Chord length [m]
    %% Run for different velocities [m/s]
    for velocities = 21
        close all
        clc
        
        % Generate airfoil
        [X,Y] = generateNACA4(foil,N);
        X = flip(X);
        Y = flip(Y);
        Y(1) = 0;
        Y(end) = 0;
        X = X*c;
        Y = Y*c;
        
        % Get the mids and normals of the geometry
        [~,~,X_mid,Y_mid,norms] = inviscidPanel(X,Y,1,alpha);
        
        rho = 1.225;                % Freestream Density [kg/m^3]
        mu = 1.802e-5;              % Dynamic Viscosity [kg/m/s]
        nu = mu/rho;                % Kinematic Viscocity [m^2/s]
        Vinf = velocities;          % Freestream velocity [m/s]
        Re = Vinf*c/nu;             % Reynolds Number
        b = 5/12/3.28;              % Span [m]
        q_inf = 0.5*rho*Vinf^2;     % Dynamic Pressure [Pa]
        
        
        %% Run a viscous panel method at this AoA
        [Cp,Cf,~,~,~,~,delta,delta_star] = Blasius_Panel(X,Y,Vinf,alpha,mu,rho,50,false,false);
        
        % Convert the Cf from vector to magnitude format
        for i = 1:length(Cf)
            Cf(i,1) = norm(Cf(i,:));
        end
        Cf(:,2) = [];
        
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
        
        % Calculate aerodynamic coefficients at this AoA
        Ca = 0;         % Normal coefficient
        Cn = 0;         % Axial coefficient
        for j = 1:length(Cp)/2
            Ca = Ca - (Cp(N-j+1)*dy_dx(N-j+1) - Cp(j)*dy_dx(j))*dx(j) + (Cf(N-j+1) + Cf(j))*dx(j);
            Cn = Cn - (Cp(j) - Cp(N-j+1))*dx(j) + (Cf(N-j+1)*dy_dx(N-j+1) + Cf(j)*dy_dx(j))*dx(j);
        end
        Cn = Cn/c;
        Ca = Ca/c;
        
        Cm = Cp'*(dx'.*X_mid + dy'.*Y_mid);      % Viscous moment coefficient
        Cd = Ca*cos(alpha) + Cn*sin(alpha);      % Viscous drag coefficient
        Cl = Cn*cos(alpha) - Ca*sin(alpha);      % Viscous lift coefficient

        
        %% Find the force on the end of the airfoil
        
        % Number of points to use on the trailing edge...
        top_n = 76; %... on the top
        bot_n = 76; %... on the bottom
        
        F_dist = zeros(top_n+bot_n-2,2);    % Force distribution [N]
        x_mids = zeros(top_n+bot_n-2,1);    % Panel centers in region of interest (x)
        y_mids = x_mids;                    % Panel centers "   "   "   "   "   " (y)
        CPs = x_mids;                       % Pressure coefficients "   "   "   "   "   "
        dxs = x_mids;                       % Delta x "   "   "   "   "   "
        dys = x_mids;                       % Delta y "   "   "   "   "   "
        iii = 1;                            % Iteration variable
        
        % Get bottom force distribution
        for i = 1:bot_n-1
            CPs(iii) = Cp(i);
            F_dist(iii,1) = -Cp(i)*dy(i)*q_inf*b + Cf(i)*dx(i)*q_inf*b;
            F_dist(iii,2) = Cp(i)*dx(i)*q_inf*b + Cf(i)*dy(i)*q_inf*b;
            x_mids(iii) = (X(i+1) + X(i))/2;
            y_mids(iii) = (Y(i+1) + Y(i))/2;
            dxs(iii) = dx(i);
            dys(iii) = dy(i);
            iii = iii + 1;
        end
        % Get top force distribution
        for i = N-top_n+1:N
            CPs(iii) = Cp(i);
            F_dist(iii,1) = -Cp(i)*dy(i)*q_inf*b + Cf(i)*dx(i)*q_inf*b;
            F_dist(iii,2) = Cp(i)*dx(i)*q_inf*b + Cf(i)*dy(i)*q_inf*b;
            x_mids(iii) = (X(i+1) + X(i))/2;
            y_mids(iii) = (Y(i+1) + Y(i))/2;
            dxs(iii) = dx(i);
            dys(iii) = dy(i);
            iii = iii + 1;
        end
        
        % Get total force on region
        Fx = sum(F_dist(:,1));
        Fy = sum(F_dist(:,2));
        F = sqrt(Fx^2 + Fy^2);
        
        % Get center of pressure in region
        Cp_1 = sum(x_mids.*abs(CPs).*dxs);
        Cp_2 = sum(abs(CPs).*dxs);
        x_cp = Cp_1/Cp_2;
        Cp_1 = sum(y_mids.*abs(CPs).*dys);
        Cp_2 = sum(abs(CPs).*dys);
        y_cp = Cp_1/Cp_2;
        
        if showPlots
            % Plot the total force vector
            figure()
            plot(X,Y,'k.-')     % Geometry
            pbaspect([1.4 1.0 1])
            xlim([-.2*c c+.2*c])
            ylim([-.5*c .5*c])
            title(sprintf("NACA %s with Local Force (Blasius)",foil),'interpreter','latex','fontsize',20)
            xlabel("x/c",'interpreter','latex','FontSize',16)
            ylabel("y/c",'interpreter','latex','FontSize',16)
            hold on
            plot(delta(:,1),delta(:,2),'b')             % Boundary Layer
            quiver(x_cp,y_cp,Fx,Fy,'color','r','linewidth',2)        % Force
            plot([X(end-top_n+1:end) X(1:bot_n) X(end-top_n+1)],[Y(end-top_n+1:end) Y(1:bot_n) Y(end-top_n+1)],'r')   % Region of Interest
            hold off
            legend('Geometry','Boundary Layer','Force','Region of Interest','location','southeast','interpreter','latex','fontsize',12)
            set(gcf, 'PaperPositionMode', 'manual')
            set(gcf, 'Color', [1 1 1])
            set(gca, 'Color', [1 1 1])
            set(gcf, 'PaperUnits', 'centimeters')
            set(gcf, 'PaperSize', [15 15])
            set(gcf, 'Units', 'centimeters' )
            set(gcf, 'Position', [0 0 15 15])
            set(gcf, 'PaperPosition', [0 0 15 15])
            
            % Plot the force distribution on the panels
            figure()
            plot(X,Y,'k.-')     % Geometry
            pbaspect([1.4 1.0 1])
            xlim([-.2*c c+.2*c])
            ylim([-.5*c .5*c])
            title(sprintf("NACA %s with Force Distribution",foil),'interpreter','latex','fontsize',20)
            xlabel("x/c",'interpreter','latex','FontSize',16)
            ylabel("y/c",'interpreter','latex','FontSize',16)
            hold on
            quiver(x_mids,y_mids,F_dist(:,1),F_dist(:,2),0.5,'color','r')        % Forces
            hold off
            set(gcf, 'PaperPositionMode', 'manual')
            set(gcf, 'Color', [1 1 1])
            set(gca, 'Color', [1 1 1])
            set(gcf, 'PaperUnits', 'centimeters')
            set(gcf, 'PaperSize', [15 15])
            set(gcf, 'Units', 'centimeters' )
            set(gcf, 'Position', [0 0 15 15])
            set(gcf, 'PaperPosition', [0 0 15 15])
        end
        
        % Output the simulation parameters and results
        fprintf("Reynolds Number: %0.0f\n", Re)
        fprintf("Alpha: %0.2f deg\n", rad2deg(alpha))
        fprintf("Force on region: %0.6f N\n", F)
        fprintf("\nForce Coefficients:\n")
        fprintf("    Cl: %0.3f\n", Cl)
        fprintf("    Cd: %0.5f\n", Cd)
    end
end

fprintf("Done! \n")
