#include <SFML/Graphics.hpp>

int w = 1280;
int h = 720;

class Cercle {
private:
    sf::CircleShape c;
public:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float r;
    float m;
    Cercle(sf::Vector2f position, sf::Color color,float radius) :pos(position),r(radius) {
        m = 10 * r;
        c.setRadius(r);
        c.setOrigin(sf::Vector2f(r, r));
        c.setFillColor(sf::Color(color));
        float angle = (float)(rand() % 360) * 3.1415f / 180.f;
        float speed = (float)(rand()%100)/100;
        vel = sf::Vector2f(cos(angle) * speed, sin(angle) * speed);
    }
    void updatePos(float &dt) {
        if (pos.y < r){
            vel.y = -vel.y;
            pos.y += r - pos.y;
        }else if (pos.x < r) {
            vel.x = -vel.x;
            pos.x += r - pos.x;
        }else if (pos.y>h - r) {
            vel.y = -vel.y;
            pos.y += h-r-pos.y;
        }
        else if (pos.x>w - r) {
            vel.x = -vel.x;
            pos.x += w-r-pos.x;
        }
        
        
        pos += vel*dt*100.0f;
    }
    void render(sf::RenderWindow& win) {
        c.setPosition(pos);
        win.draw(c);
    }
};

class System {
private:
    std::vector<Cercle> liste;
public:
    System(int number) {
        for (int i = 0; i < number; i++) {
            liste.push_back(Cercle(sf::Vector2f(rand()%w,rand()%h), sf::Color(50, i*50,255),(rand()%50)+5));
        }
    }
    void update(sf::RenderWindow &win, float &dt) {
        for (auto& c : liste) {
            for (auto& c1 : liste) {
                if (&c != &c1) {
                    float dx = c1.pos.x - c.pos.x;
                    float dy = c1.pos.y - c.pos.y;
                    float d = sqrt(dx * dx + dy * dy);
                    float rd = c.r + c1.r;
                    if (d<=rd) {

                        float profondeur = rd - d;

                        float nx = dx / d;
                        float ny = dy / d;

                        float tx = -ny;
                        float ty = nx;

                        float dpt = c.vel.x * tx + c.vel.y * ty;
                        float dpt1 = c1.vel.x * tx + c1.vel.y * ty;

                        float dpn = c.vel.x * nx + c.vel.y * ny;
                        float dpn1 = c1.vel.x * nx + c1.vel.y * ny;

                        float m = (dpn * (c.m - c1.m) + 2.0f * c1.m * dpn1) / (c.m+c1.m);
                        float m1 = (dpn1 * (c1.m - c.m) + 2.0f * c.m * dpn) / (c.m + c1.m);

                        c.pos.x += -nx * 0.5 * profondeur;
                        c.pos.y += -ny * 0.5 * profondeur;
                        c1.pos.y += nx * 0.5 * profondeur;
                        c1.pos.x += ny * 0.5 * profondeur;

                        c.vel.x = tx * dpt +nx*-abs(m);
                        c.vel.y = ty * dpt +ny*-abs(m);
                        c1.vel.x = tx * dpt1+nx*abs(m1);
                        c1.vel.y = ty * dpt1+ny*abs(m1);
                    }

                }
            }
            c.updatePos(dt);
            c.render(win);
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(w, h), "collision");

    System sys(100);
    sf::Clock clock;
    float dt;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        dt = clock.restart().asSeconds();

        window.clear();

        sys.update(window,dt);

        window.display();
    }

    return 0;
}
