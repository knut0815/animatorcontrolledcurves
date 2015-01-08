/* CSC 473 Fall 2014 - Assignment 2
 * Ryan Guy
 */

#include "cameramodel.h"

CameraModel::CameraModel()
{
    alpha = 1.0;
}

void CameraModel::setAlpha(float val) {
    alpha = val;
}

void CameraModel::draw() {
    float w1 = 1.0;
    float w2 = 0.75;
    float h1 = 0.5;
    float h2 = 1.0;
    float d1 = -0.75;

    glm::vec3 v1 = glm::vec3(-0.5*w1, 0.5*h1, 0.5*d1);
    glm::vec3 v2 = v1 + glm::vec3(w1, 0.0, 0.0);
    glm::vec3 v3 = v1 - glm::vec3(0.0, 0.0, d1);
    glm::vec3 v4 = v1 + glm::vec3(w1, 0.0, -d1);
    glm::vec3 v5 = v1 - glm::vec3(0.0, h1, 0.0);
    glm::vec3 v6 = v2 - glm::vec3(0.0, h1, 0.0);
    glm::vec3 v7 = v4 - glm::vec3(0.0, h1, 0.0);
    glm::vec3 v8 = v3 - glm::vec3(0.0, h1, 0.0);
    glm::vec3 v9 = glm::vec3(0.5*w1, 0.0, 0.0);
    glm::vec3 v10 = v2 + glm::vec3(w2, 0.0, 0.0);
    glm::vec3 v11 = v4 + glm::vec3(w2, 0.0, 0.0);
    glm::vec3 v12 = v6 + glm::vec3(w2, 0.0, 0.0);
    glm::vec3 v13 = v7 + glm::vec3(w2, 0.0, 0.0);
    glm::vec3 v14 = glm::vec3(0.0, 0.5*h1 + h2, 0.0);

    glBegin(GL_TRIANGLES);

    glColor4f(0.0, 0.0, 0.73750, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v14.x, v14.y, v14.z);
    glVertex3f(v3.x, v3.y, v3.z);

    glColor4f(0.0, 0.0, 0.38840, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v14.x, v14.y, v14.z);

    glColor4f(0.0, 0.0, 0.847260, alpha);
    glVertex3f(v14.x, v14.y, v14.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v4.x, v4.y, v4.z);

    glColor4f(0.0, 0.0, 0.2738, alpha);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v14.x, v14.y, v14.z);
    glVertex3f(v4.x, v4.y, v4.z);

    glColor4f(0.0, 0.0, 0.53620, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v8.x, v8.y, v8.z);

    glColor4f(0.0, 0.0, 0.737320, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v5.x, v5.y, v5.z);
    glVertex3f(v8.x, v8.y, v8.z);

    glColor4f(0.0, 0.0, 0.26730, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v6.x, v6.y, v6.z);

    glColor4f(0.0, 0.0, 0.277190, alpha);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v6.x, v6.y, v6.z);
    glVertex3f(v5.x, v5.y, v5.z);

    glColor4f(0., 0.0, 0.83770, alpha);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glVertex3f(v7.x, v7.y, v7.z);

    glColor4f(0.0, 0.0, 0.73760, alpha);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v7.x, v7.y, v7.z);
    glVertex3f(v8.x, v8.y, v8.z);

    glColor4f(0.0, 0.0, 0.94880, alpha);
    glVertex3f(v8.x, v8.y, v8.z);
    glVertex3f(v5.x, v5.y, v5.z);
    glVertex3f(v6.x, v6.y, v6.z);

    glColor4f(0.0, 0.0, 0.828930, alpha);
    glVertex3f(v8.x, v8.y, v8.z);
    glVertex3f(v6.x, v6.y, v6.z);
    glVertex3f(v7.x, v7.y, v7.z);

    glColor4f(0.0, 0.0, 0.93830, alpha);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v6.x, v6.y, v6.z);
    glVertex3f(v7.x, v7.y, v7.z);

    glColor4f(0.0, 0.0, 0.9390, alpha);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v7.x, v7.y, v7.z);
    glVertex3f(v4.x, v4.y, v4.z);

    glColor4f(0.0, 0.0, 0.632780, alpha);
    glVertex3f(v9.x, v9.y, v9.z);
    glVertex3f(v10.x, v10.y, v10.z);
    glVertex3f(v11.x, v11.y, v11.z);

    glColor4f(0.0, 0.0, 0.7370, alpha);
    glVertex3f(v9.x, v9.y, v9.z);
    glVertex3f(v10.x, v10.y, v10.z);
    glVertex3f(v12.x, v12.y, v12.z);

    glColor4f(0.0, 0.0, 0.83730, alpha);
    glVertex3f(v9.x, v9.y, v9.z);
    glVertex3f(v12.x, v12.y, v12.z);
    glVertex3f(v13.x, v13.y, v13.z);

    glColor4f(0.0, 0.0, 0.9820, alpha);
    glVertex3f(v9.x, v9.y, v9.z);
    glVertex3f(v13.x, v13.y, v13.z);
    glVertex3f(v11.x, v11.y, v11.z);

    glColor4f(0.0, 0.0, 0.73830, alpha);
    glVertex3f(v10.x, v10.y, v10.z);
    glVertex3f(v13.x, v13.y, v13.z);
    glVertex3f(v11.x, v11.y, v11.z);

    glColor4f(0.0, 0.0, 0.83830, alpha);
    glVertex3f(v10.x, v10.y, v10.z);
    glVertex3f(v12.x, v12.y, v12.z);
    glVertex3f(v13.x, v13.y, v13.z);

    glEnd();


}







