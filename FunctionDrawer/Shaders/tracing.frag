#version 330 core

in vec2 pixel;
out vec3 outColor;

//Camera
uniform vec3 eye;
uniform vec3 U;
uniform vec3 W;
uniform vec3 V;

//Bounds
uniform vec3 minv;
uniform vec3 maxv;

//Tracing stuff
uniform float Lstep;
uniform float tolerance; //bisection tolerance
uniform int AAlevel;
uniform vec2 invScreenSize;
const float EPS = 0.0001; //zero

//Material
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

//Scene
uniform vec3 lightDirection;
uniform vec3 lightIntensity;
uniform vec3 missColor;

//x derivative
float Fx(in vec2 p)
{
    return #Fx;
}
// y derivative
float Fy(in vec2 p)
{
    return #Fy;
}
// G(L) = F(eye.x + L * d.x, eye.z + L * d.z) - eye.y - L * d.y
float G(in float L, in vec3 d)
{
    return #G;
}

vec3 getNormal(in vec2 p)
{
    return normalize(vec3(-Fx(p), 1.0, -Fy(p)));
}

//get min and max lambda
bool intersectAABB(in vec3 d, out float Lmin, out float Lmax)
{
    vec3 tmin = (minv - eye) / d;
    vec3 tmax = (maxv - eye) / d;
    
    vec3 near = min(tmin, tmax);
    vec3 far = max(tmin, tmax);
    Lmin = max(max(near.x, near.y), near.z);
    Lmin = max(0.0, Lmin);
    Lmax = min(min(far.x, far.y), far.z);
    
    return Lmax >= Lmin;
}

float bisection(in float a, in float Ga, in float b, in vec3 d)
{
    while(b - a > tolerance)
    {
        float c = (a + b) * 0.5; // new midpoint
        float Gc = G(c, d);
        
        if(abs(Gc) < EPS)
            return c;

        if(Gc * Ga > 0.0)
        {
            a = c;
            Ga = Gc;
        }
        else 
            b = c;
    }
    return (a + b) * 0.5;
}

bool intersectFunction(in vec3 d, in float Lmin, in float Lmax, out vec3 intersection_point)
{
    float a = Lmin;
    float Ga = G(a, d);
    for(float L = Lmin + Lstep; L < Lmax; L += Lstep)
    {
        float b = L;
        float Gb = G(b, d);
        
        if(Ga * Gb < 0.0)
        {
            float iL = bisection(a, Ga, b, d);
            intersection_point = eye + iL * d;
            return true;
        }
            
        a = b;
        Ga = Gb;
    }
    return false;
}

void main()
{
    outColor = vec3(0.0);
    float invAA = 1.0 / float(AAlevel);
    for(int i = 0; i < AAlevel; ++i)
        for(int j = 0; j < AAlevel; ++j)
        {
            vec2 d = pixel + 2.0 * invScreenSize * invAA * vec2(i, j);
            vec3 rayDir = normalize(d.x * U + d.y * V + W);
            vec3 color = missColor;
            
            float Lmin;
            float Lmax;
            if(intersectAABB(rayDir, Lmin, Lmax))
            {
                vec3 position;
                if(intersectFunction(rayDir, Lmin, Lmax, position))
                {
                    vec3 normalDirection = getNormal(position.xz);
                    float dotNL = dot(normalDirection, lightDirection);
                    
                    color = ambient;
                    if(dotNL > 0.0)
                    {
                        vec3 diffuseReflection = diffuse * dotNL;
                        vec3 viewDirection = normalize(eye - position);
                        vec3 specularReflection = specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), shininess);
                        color += lightIntensity * (diffuseReflection + specularReflection);
                    }
                }
            }
            outColor += color;
        }
        
    outColor *= invAA * invAA;
}
