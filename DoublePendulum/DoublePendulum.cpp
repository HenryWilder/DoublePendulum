#include <vector>
#include <initializer_list>
#include <raylib.h>
#include <raymath.h>

Vector2 Vector2FromLengthAndAngle(Vector2 point, float offsetLength, float angle)
{
    Vector2 result =
    {
        point.x + cosf(angle) * offsetLength,
        point.y + sinf(angle) * offsetLength
    };

    return result;
}

void DrawPointV(Vector2 point, float radius, Color color)
{
    float diameter = radius * 2;
    Rectangle rec 
    {
        point.x - radius,
        point.y - radius,
        diameter,
        diameter
    };
    DrawRectangleRec(rec, color);
}

struct Segment
{
public:
    Segment(float length, float angle)
        : length(length), angle(angle), endpoint() {}

    const float length;
    float angle;
    Vector2 endpoint;
};

struct Weight
{
public:
    Weight(float mass, float radius)
        : mass(mass), radius(radius) {}

    const float mass;
    const float radius;
};

class Pendulum
{
private:
    Vector2 anchor;
    Weight weight;
    std::vector<Segment> segments;

    void UpdateEndpoints()
    {
        Vector2 parent = anchor;
        for (Segment& seg : segments)
        {
            seg.endpoint = 
            {
                parent.x + cosf(seg.angle) * seg.length,
                parent.y + sinf(seg.angle) * seg.length
            };
            parent = seg.endpoint;
        }
    }

public:

    Pendulum(Vector2 anchor, Weight weight, std::initializer_list<Segment> segments)
        : anchor(anchor), weight(weight), segments(segments)
    {
        UpdateEndpoints();
    }

    void Update()
    {

        UpdateEndpoints();
    }

    void Draw() const
    {
        Vector2 parent = anchor;
        for (Segment seg : segments)
        {
            DrawLineV(parent, seg.endpoint, WHITE);
            parent = seg.endpoint;
        }

        DrawCircleV(parent, weight.radius, WHITE);

        DrawPointV(anchor, 2, RED);
        for (Segment seg : segments)
        {
            DrawPointV(seg.endpoint, 2, GREEN);
        }
    }
};

int main()
{
    int windowWidth = 1280;
    int windowHeight = 720;
    InitWindow(windowWidth, windowHeight, "Double Pendulum");

    SetTargetFPS(280);

    Pendulum pendulum = Pendulum(
        Vector2{
            windowWidth  * 0.5f,
            windowHeight * 0.5f
        },
        Weight(20, 20),
        {
            Segment(100, 0),
            Segment(100, 0),
        }
    );

    while (!WindowShouldClose())
    {
        pendulum.Update();

        BeginDrawing();

        ClearBackground(BLACK);

        pendulum.Draw();

        EndDrawing();
    }

    CloseWindow();
}
