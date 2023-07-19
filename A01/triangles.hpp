

void SetupTriangles(Assignment01 *A) {
  /**************
   Function Triangle(float x1, float y1, float x2, float y2, float x3, float y3,
  float r, float g, float b)

   draws a triangle connecting points (x1,y1)  (x2, y2) and (x3, y3), colored
  with the (r,g,b) specified color. vertex are expressed in Normalized screen
  coordinates, (i.e. in range [-1,+1]), following the Vulkan convention. The
  red, green and blue (r,g,b) values are in the range [0,1].

   Using function Triangle(...) try to draw a children house, similar to the one
  below.

   First remove the two function below (given only as an example), and replace
  them with your own. Since it is a C program, you can use for loops if you
  think it can be helpful in your drawing. The for() command below that draws a
  sun like shape in exact center of the screen near the top, is an example on
  how you can use it: please remove it, and replace it with your own code. It is
  not necessery to use a for loop if you do not really need it: it has been put
  here only as an example to help students not familiar with C++.
  ***************/
  // tail
  A->Triangle(-0.80, 0.60, -0.65, 0.85, -0.45, 0.60, 0, 1, 0);
  A->Triangle(-0.25, 0.85, -0.65, 0.85, -0.45, 0.60, 0, 1, 0);
  // bondy
  A->Triangle(-0.25, 0.85, -0.25, 0.12, 0.25, 0.85, 0, 0, 1);
  A->Triangle(0.12, -0.40, -0.25, 0.12, 0.12, 0.66, 1, 0, 0);
  A->Triangle(0.12, -0.40, 0.12, 0.35, 0.38, -0.05, 1, 1, 0);
  // Head
  A->Triangle(0.12, -0.40, 0.48, -0.40, 0.30, -0.15, 1, 0, 1);
  A->Triangle(0.12, -0.40, 0.48, -0.40, 0.30, -0.65, 1, 0, 1);
  A->Triangle(0.12, -0.40, 0.12, -0.90, 0.30, -0.65, 1, 0.75, 1);
  A->Triangle(0.48, -0.90, 0.48, -0.40, 0.30, -0.65, 0.5, 1, 1);
}
