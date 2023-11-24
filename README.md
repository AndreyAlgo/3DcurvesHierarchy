# 3DcurvesHierarchy

This ia small program in C++ that implements support of 3D curves hierarchy.
1. Support a few types of 3D geometric curves – circles, ellipses and 3D helixes. Each curve returns a 3D point and a first derivative (3D vector) per parameter t along the curve.
2. Populates a container (e.g. vector or list) of objects of these types created in random manner with random parameters.
3. Prints coordinates of points and derivatives of all curves in the container at t=PI/4.
4. Populates a second container that would contain only circles from the first container. Make sure the second container shares (i.e. not clones) circles of the first one, e.g. via pointers.
5. Sorts the second container in the ascending order of circles’ radii. That is, the first element has the smallest radius, the last - the greatest.
6. Computes the total sum of radii of all curves in the second container.

Helix is spatial and is defined by its radius and step (see the figure below). It takes 2 * PI in parametric space to make a round, i.e. any point on helix satisfies the condition C(t + 2*PI) = C(t) + {0, 0, step}.

Boost 1.39 
