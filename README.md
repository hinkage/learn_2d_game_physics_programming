
学习pikuma的课程：2D Game Physics Programming。

## Proof of Rotation Matrix
<img src="./assets/images/proof_rotation_matrix.png" height="450">

## Integration Method
Implicit Euler: Best for stiff systems due to unconditional stability, but requires solving a system of equations at each step.

Verlet Integration: Excellent for conserving energy and handling physical systems with constant or smoothly varying forces, but generally only second-order accurate.

Runge-Kutta Methods: Highly accurate and flexible, suitable for a wide range of problems, but can suffer from stability issues and require more computational effort per step.

## Drag Force
$$F_d = \frac{1}{2} \rho v^2 C_d A * -\hat{v}$$
ρ (rho) is the density of the fluid (kg/m³).

v is the velocity of the body relative to the fluid (m/s).

𝐶_𝑑 is the drag coefficient, which is a dimensionless number that depends on the shape of the body and the flow conditions.

A is the reference area of the body (m²), often the frontal area for objects moving through the air.

## Friction Force
### Static Friction
$$F_s \leq \mu_s N$$
Static friction acts on objects that are not moving relative to each other.
F_s is the static friction force.

μ_s is the coefficient of static friction, a dimensionless value that depends on the materials in contact.

N is the normal force, which is the perpendicular force exerted by a surface on an object (usually equal to the object's weight if the surface is horizontal).

The static friction force adjusts to match the applied force up to a maximum value. If the applied force exceeds this maximum, the object starts to move, and kinetic friction takes over.
### Kinetic Friction
$$F_k = \mu_k N$$

Kinetic friction acts on objects that are moving relative to each other. 

F_k is the kinetic friction force.

μ_k is the coefficient of kinetic friction, which is usually less than the coefficient of static friction.

N is the normal force.

## Newton's Law of Universal Gravitation
$$F_g = G \frac{m_1 m_2}{r^2}$$

## Spring Force
$$F_s = -k x$$

