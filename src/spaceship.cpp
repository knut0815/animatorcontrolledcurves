/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "spaceship.h"

SpaceShip::SpaceShip()
{
    alpha = 1.0;
}

void SpaceShip::setAlpha(float val) {
    alpha = val;
}

void SpaceShip::draw() {
    float h1 = 2.0;
    float h2 = 0.25 * h1;
    float h3 = 0.5;
    float h4 = 0.5;
    float w1 = 0.75;
    float w2 = 0.75;
    float w3 = 0.25;
    float d = 1.0;

    glm::vec3 v1 = glm::vec3(0.5*h1, 0.0, 0.5*w1);
    glm::vec3 v2 = v1 - glm::vec3(h1, 0.0, 0.0);
    glm::vec3 v3 = v1 - glm::vec3(h1, 0.0, w1);
    glm::vec3 v4 = v1 - glm::vec3(0.0, 0.0, w1);
    glm::vec3 v5 = v2 + glm::vec3(h2, d, -0.5*w1);
    glm::vec3 v6 = glm::vec3(0.5*h1+h4, 0.0, 0.0);
    glm::vec3 v7 = v2 + glm::vec3(h2+h3, 0.0, 0.0);
    glm::vec3 v8 = v7 + glm::vec3(0.0, 0.0, w2);
    glm::vec3 v9 = v7 + glm::vec3(-h3, 0.0, w2);
    glm::vec3 v10 = v7 - glm::vec3(h3, 0.0, 0.0);
    glm::vec3 v11 = v9 + glm::vec3(0.5*h3, 0.25*d, w3);
    glm::vec3 v12 = v3 + glm::vec3(h2+h3, 0.0, 0.0);
    glm::vec3 v13 = v3 + glm::vec3(h2, 0.0, 0.0);
    glm::vec3 v14 = v13 - glm::vec3(0.0, 0.0, w2);
    glm::vec3 v15 = v12 - glm::vec3(0.0, 0.0, w2);
    glm::vec3 v16 = v14 + glm::vec3(0.5*h3, 0.25*d, -w3);

    glBegin(GL_TRIANGLES);

    glColor4f(0.3884, 0.0, 0.0, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);

    glColor4f(0.5837, 0.0, 0.0, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);

    glColor4f(0.174, 0.0, 0.0, alpha);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v5.x, v5.y, v5.z);

    glColor4f(0.96783, 0.0, 0.0, alpha);
    glVertex3f(v4.x, v4.y, v4.z);
    glVertex3f(v5.x, v5.y, v5.z);
    glVertex3f(v3.x, v3.y, v3.z);

    glColor4f(0.573, 0.0, 0.0, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v5.x, v5.y, v5.z);
    glVertex3f(v4.x, v4.y, v4.z);

    glColor4f(0.2734, 0.0, 0.0, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v5.x, v5.y, v5.z);

    glColor4f(0.8684, 0.0, 0.0, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glVertex3f(v6.x, v6.y, v6.z);

    glColor4f(0.867, 0.0, 0.0, alpha);
    glVertex3f(v7.x, v7.y, v7.z);
    glVertex3f(v8.x, v8.y, v8.z);
    glVertex3f(v10.x, v10.y, v10.z);

    glColor4f(0.4658, 0.0, 0.0, alpha);
    glVertex3f(v8.x, v8.y, v8.z);
    glVertex3f(v9.x, v9.y, v9.z);
    glVertex3f(v10.x, v10.y, v10.z);

    glColor4f(0.7573, 0.0, 0.0, alpha);
    glVertex3f(v8.x, v8.y, v8.z);
    glVertex3f(v11.x, v11.y, v11.z);
    glVertex3f(v9.x, v9.y, v9.z);

    glColor4f(0.3647, 0.0, 0.0, alpha);
    glVertex3f(v12.x, v12.y, v12.z);
    glVertex3f(v13.x, v13.y, v13.z);
    glVertex3f(v15.x, v15.y, v15.z);

    glColor4f(0.673, 0.0, 0.0, alpha);
    glVertex3f(v13.x, v13.y, v13.z);
    glVertex3f(v14.x, v14.y, v14.z);
    glVertex3f(v15.x, v15.y, v15.z);

    glColor4f(0.475, 0.0, 0.0, alpha);
    glVertex3f(v14.x, v14.y, v14.z);
    glVertex3f(v16.x, v16.y, v16.z);
    glVertex3f(v15.x, v15.y, v15.z);

    glEnd();

}







