# FoilAnalyzer
 
A C++ program using wxWidgets which can generate and plot NACA 4 and 5 digit series airfoils with a specified number of panels. The goal of the project is to perform potential flow and viscous calculations on the airfoils for specified Reynold's numbers, densities, velocities, and angles of attack. The dimensionless coefficients (e.g. section coefficient of lift) can then be plotted against angle of attack. The pressure distribution across the top and bottom surfaces will also be plottable.

## Airfoil Viewer

![Airfoil Viewer](/images/AirfoilViewer1.PNG)

Airfoils can be generated with the "Define Airfoil" button on the bottom left where the user can input a 4 or 5 digit NACA code and the number of panels. Loaded airfoil information can be viewed at the bottom where the user can select the plot color, whether to show it or not, and delete it.

![Airfoil Viewer Airfoils](/images/AirfoilViewer2.PNG)
