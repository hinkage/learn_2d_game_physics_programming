Learn course from:

https://pikuma.com/courses/game-physics-engine-programming

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

## Simulating Soft Body
<img src="./assets/images/simulating_soft_body.png">

Add spring force beteen 4 particles.

## Angular Velocity

$$\omega = \frac{d\theta}{dt}$$

<img src="./assets/images/angular_velocity.png" height="400">

## Torque Force

$$\tau = r F \sin(\theta)$$

Simplify it to

$$\tau = I \alpha$$

I is Moment of Inertia, which measures how much an object "resists" to change its angular acceleration.

https://en.wikipedia.org/wiki/List_of_moments_of_inertia

## Collision Detection
### Collision Contact Information
<img src="./assets/images/collision_contact_information.png">

### Broad Phase and Narrow Phase
Collision detection is crucial in ensuring realistic interactions in simulations and games. The broad phase quickly culls non-colliding pairs, making the narrow phase, which involves detailed and computationally expensive checks, more efficient. Together, these phases ensure accurate and efficient collision detection.

### Collision Response
The collision normal is the direction in which the potential impulse should be applied.
The penetration depth (along with some other things) will determine how big of an impulse we need to use.

Projection Method

The Projection method, also known as the Position Correction method, involves directly adjusting the positions of colliding objects to resolve interpenetration. This method is typically used in conjunction with other collision resolution techniques to ensure that objects do not overlap.

$$\mathbf{MTV} = d \mathbf{n}$$

$$\Delta \mathbf{r}_1 = -\frac{m_2}{m_1 + m_2} \mathbf{MTV}$$

$$\Delta \mathbf{r}_2 = \frac{m_1}{m_1 + m_2} \mathbf{MTV}$$

Minimum Translation Vector(MTV).

n as the normal vector at the point of contact pointing from object 1 to object 2.

d as the penetration depth.

Impulse Method

The Impulse method resolves collisions by applying an instantaneous change in velocity (impulse) to the colliding objects, based on the laws of conservation of momentum and energy.

$$\mathbf{J} = -(1 + \epsilon) \frac{\mathbf{v}_r \cdot \mathbf{\hat{n}}}{\frac{1}{m_1} + \frac{1}{m_2}} $$

J is the impulse.

$\epsilon$ is the coefficient of restitution (0 for perfectly inelastic, 1 for perfectly elastic collisions).

v_r  is the relative velocity.

n is the collision normal.

m_1 and m_2 are the masses of the colliding objects.

<img src="./assets/images/derive_impluse_method.png">

Penalty Method

The Penalty method simulates collisions by applying a repulsive force proportional to the depth of penetration and possibly the relative velocity of the objects. This method treats interpenetration as a soft constraint.

$$\mathbf{F} = -k \mathbf{d} - b \mathbf{v}$$

F is the penalty force.

k is the stiffness constant (spring constant).

d is the penetration depth vector.

b is the damping coefficient.

v is the relative velocity.

#### Object with Infinite Mass
```cpp
bool Body::IsStatic() {
    float epsilon = 0.005f;
    return fabs(invMass - 0.0f) < epsilon;
}
```
#### Momentum
Elastic Collisions

$$m_1 v_{1,\text{initial}} + m_2 v_{2,\text{initial}} = m_1 v_{1,\text{final}} + m_2 v_{2,\text{final}}$$

$$\frac{1}{2} m_1 v_{1,\text{initial}}^2 + \frac{1}{2} m_2 v_{2,\text{initial}}^2 = \frac{1}{2} m_1 v_{1,\text{final}}^2 + \frac{1}{2} m_2 v_{2,\text{final}}^2$$

Inelastic Collisions

$$m_1 v_{1,\text{initial}} + m_2 v_{2,\text{initial}} = (m_1 + m_2) v_{\text{final}}$$

#### Linear Collision Response
<img src="./assets/images/linear_collision_response.png" height="350">

Linear collision response focuses on updating the linear velocities of colliding bodies without considering rotational effects. This is a simplified scenario compared to full rigid body dynamics but is commonly used in many simulations where rotational effects are negligible.

### Axis-Aligned Bounding Box
<img src="./assets/images/AABB.png" height="500">

### Separating Axis Theorem
The Separating Axis Theorem (SAT) is a fundamental concept in computational geometry, particularly useful in collision detection algorithms for convex shapes. The theorem states that two convex shapes are not intersecting if and only if there exists a line (axis) along which the projections of the two shapes do not overlap.

<img src="./assets/images/convex_concave.png" height="300">

<img src="./assets/images/SAT.png" height="400">

点到edge normal的投影，就是点到edge的垂直距离。

<img src="./assets/images/polygon_polygon_contact.png" height="400">

当y轴指向上方时，normal的计算是顺时针旋转。但是SDL的坐标系，y轴正方向指向下方，此时normal就是逆时针旋转90度。

所以a的normal是指向a外边的，如果b有一点在a内部，那么dot product就是负数。

<img src="./assets/images/polygon_polygon_contact_result.png">

### Linear and Angular Impulse
<img src="./assets/images/linear_and_angular_velocity_after_collision.png">
<img src="./assets/images/linear_and_angular_velocity_after_collision_2.png">

$\omega$ is perpendicular to 2D screen.

cross($\omega$, r) = cross(Vec3(0, 0, $\omega$), Vec3(rx, ry, 0)) = Vec3(0*0-$\omega$*ry, $\omega$\*rx-0\*0, 0)

<img src="./assets/images/simplification_by_triple_scalar_product_rule.png">

<img src="./assets/images/linear_and_angular_impulse_result.png">

<img src="./assets/images/multiple_vertices_polygon.png">

### Local Solvers and Global Solvers
Local solvers focus on solving collision response for specific parts or segments of a system. They are particularly useful for handling smaller, isolated interactions and are often more efficient for real-time applications.

Global solvers aim to find a solution that considers the entire system as a whole. They are more comprehensive and can handle complex interactions across the entire system but are often more computationally intensive. 

## Constrained Rigid-Body Physics
Join constraints.

Non-penetration constraints.

Constraints are rules the must be satisfied.

Constraints remove "degrees of freedom".

### Floor Collision Velocity Constraint

$$v_y=-\frac{\beta}{dt}*c$$

Baumgarte stabilization factor.

### Constraint Force
Since the external forces acting on our system are known (weight, wind, drag, etc.),
the question now is how to calculate the **internal force** required to keep the
constraint valid, regardless of the external forces that are applied.

Let's say we know the function that gives an object's **position over time**.
(this can be a circular path, or a curve path, etc.)

We are insterested in how this constraint **changes over time**.

### Impulse VS Force Based Constraints

<img src="./assets/images/impulse_force_constraints.png" height="400">

### Generalized Velocity-Based Constraint

<img src="./assets/images/velocity_based_constraints.png" height="400">

<img src="./assets/images/Jacobian.png" height="400">

<img src="./assets/images/Jacobian2.png" height="400">

The Jacobian matrix is a fundamental concept in vector calculus, particularly useful in the context of multivariable functions. It is used to describe the rate of change of a vector-valued function with respect to its variables. In simpler terms, the Jacobian matrix generalizes the concept of a derivative for multivariable functions.

### Deriving the Distance Jacobian

https://math.stackexchange.com/questions/2207608/proof-for-derivative-of-dot-product

https://proofwiki.org/wiki/Derivative_of_Dot_Product_of_Vector-Valued_Functions

<img src="./assets/images/derivation_of_dot_product.png" height="600">

<img src="./assets/images/distance_Jacobian.png" height="500">

https://mathinsight.org/scalar_triple_product

<img src="./assets/images/distance_Jacobian2.png" height="550">

### Ax=B
Gauss-Seidel Method

### Warm Start
Warm Start refers to the practice of using the results (specifically, the constraint impulses or forces) from the previous simulation step as an initial guess for the current simulation step's constraint solver. This approach leverages the temporal coherence of physical simulations, where the state of the system typically changes only slightly between consecutive steps.

<img src="./assets/images/join_constraint.png">

Ragdoll with Joint Constraints

<img src="./assets/images/ragdoll.png">

<img src="./assets/images/ragdoll2.png">

### Penetration Constraint

<img src="./assets/images/derive_penetration_Jacobian.png" height="600">

这个图normal的方向和前面的不一致，前面的是正确的。

## Multiple Contact Points

https://github.com/erincatto/box2d-lite/blob/master/docs/GDC2006_Catto_Erin_PhysicsTutorial.pdf

<img src="./assets/images/incident_edge.png">

<img src="./assets/images/multiple_contacts.png">

<img src="./assets/images/angrybirds.png">

## Others
Contact Caching
Manifold Caching

Continuous Collison Detection

Broad and Narrow Split

Euler Integration

MidPoint and RK4 Integrators

Verlet Integration

Stick Constraints

