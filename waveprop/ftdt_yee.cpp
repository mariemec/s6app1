#include <iostream>
#include <ostream>
#include <cmath>
#include <vector>
#include <string>

// 4D vector type definition
typedef std::vector<int> Vector1D;
typedef std::vector<Vector1D> Matrix2D;
typedef std::vector<Matrix2D> Matrix3D;
typedef std::vector<Matrix3D> Matrix4D;
typedef std::tuple<int, int, int, int> Position;


void printCube(Matrix3D cube){
    for(auto slice: cube){
        for(auto row: slice){
            for(auto val: row){
                std::cout << val <<" ";
            }
            std::cout <<std::endl;
        }
        std::cout <<std::endl;
    }
}

void print4D(Matrix4D matrix){
    std::cout << "4d matrix:" << std::endl;
    bool onlyZeroes = true;
    for(auto cube: matrix){
        for(auto slice: cube){
            for(auto row: slice){
                for(auto val: row){
                    if (val != 0.0){
                        onlyZeroes = false;
                    }
                }
            }
        }
    }
    if (!onlyZeroes){
        for(auto cube: matrix){
            for(auto slice: cube){
                for(auto row: slice){
                    for(auto val: row){
                        std::cout << val << " ";
                    }
                    std::cout<<std::endl;
                }
                std::cout<< std::endl;
            }
            std::cout<< std::endl;
            std::cout<< std::endl;
        }
    }else{
        std::cout<<"matrix is filled with zeroes :("<<std::endl;
    }
}

// Used to specify the size of Matrix4D
struct Shape{
    int x;
    int y;
    int z;
    int f;
};

class Delimiter{
public:
    int begin; // the starting bound is included
    int end; // the end bound is excluded
    int span;

    Delimiter(){}

    Delimiter(int b){
        begin = b;
        end = b+1;
        span = 1;
    }

    Delimiter(int b, int e){
        begin=b;
        end=e;
        span=e-b;
    };
};

class Delimiter4D{
public:
    Delimiter x_delimiter;
    Delimiter y_delimiter;
    Delimiter z_delimiter;
    int field_component;

    Delimiter4D(){}

    Delimiter4D(Delimiter x_del, Delimiter y_del, Delimiter z_del, int index){
        x_delimiter = x_del;
        y_delimiter = y_del;
        z_delimiter = z_del;
        field_component = index;
    }
    std::string toString(){
        return "X: "+ std::to_string(x_delimiter.begin) + "-" + std::to_string(x_delimiter.end) + "(" + std::to_string(x_delimiter.span) + ")  Y: "+ std::to_string(y_delimiter.begin) + "-"+ std::to_string(y_delimiter.end)+ "(" + std::to_string(y_delimiter.span) + ")  Z: "+ std::to_string(z_delimiter.begin) + "-"+ std::to_string(z_delimiter.end)+ "(" + std::to_string(z_delimiter.span) + ")";
    }
};

Matrix3D cut_out_cube(Matrix4D matrix, Delimiter4D delimiters){
    Matrix3D cube = Matrix3D(delimiters.x_delimiter.span,Matrix2D(delimiters.y_delimiter.span,Vector1D(delimiters.z_delimiter.span)));
    int i = 0;
    int j = 0;
    int k = 0;
    std::cout<<delimiters.toString()<<std::endl;

    for (int x=delimiters.x_delimiter.begin ; x<delimiters.x_delimiter.end;x++){
        for (int y=delimiters.y_delimiter.begin ; y<delimiters.y_delimiter.end;y++){
            for (int z=delimiters.z_delimiter.begin ; z<delimiters.z_delimiter.end;z++){
                cube.at(k).at(j).at(i) = matrix.at(x).at(y).at(z).at(delimiters.field_component);
                i++;
            }
            i=0;
            j++;
        }  
        i=0;
        j=0;
        k++; 
    }

    return cube;
}

// subtracts cube2 from cube1 (in place, result is in cube1)
void subtract3D(Matrix3D* cube1, Matrix3D* cube2){
    for (int i=0;i<cube1->size();i++){
        for (int j=0;j<cube1->at(i).size();j++){
            for (int k=0;k<cube1->at(i).at(j).size();k++){
                cube1->at(i).at(j).at(k) -= cube2->at(i).at(j).at(k);
            }  
        }   
    }
}

// multiplies every value in  matrix with factor
void multiply(Matrix4D* matrix, float factor){
    for (int i=0;i<matrix->size();i++){
        for (int j=0;j<matrix->at(i).size();j++){
            for (int k=0;k<matrix->at(i).at(j).size();k++){
                for (int l=0;l<matrix->at(i).at(j).at(k).size();l++){
                    matrix->at(i).at(j).at(k).at(l) *=factor;
                }  
            }  
        }   
    }
}

// subtracts 2 4D matrices together, stores the result in the first
void subtract(Matrix4D* matrix1, Matrix4D matrix2){
    for (int i=0;i<matrix1->size();i++){
        for (int j=0;j<matrix1->at(i).size();j++){
            for (int k=0;k<matrix1->at(i).at(j).size();k++){
                for (int l=0;l<matrix1->at(i).at(j).at(k).size();l++){
                    matrix1->at(i).at(j).at(k).at(l) -= matrix2.at(i).at(j).at(k).at(l);
                }  
            }  
        }   
    }
}

// adds 2 4D matrices together, stores the result in the first
void add(Matrix4D* matrix1, Matrix4D matrix2){
    for (int i=0;i<matrix1->size();i++){
        for (int j=0;j<matrix1->at(i).size();j++){
            for (int k=0;k<matrix1->at(i).at(j).size();k++){
                for (int l=0;l<matrix1->at(i).at(j).at(k).size();l++){
                    matrix1->at(i).at(j).at(k).at(l) += matrix2.at(i).at(j).at(k).at(l);
                }  
            }  
        }   
    }
}

void add_to_4dMatrix(Matrix4D* matrix, Matrix3D sourceCube, Shape shape, Delimiter4D destDelimiters){
    int i=0;
    int j=0;
    int k=0;
            
    for (int x=destDelimiters.x_delimiter.begin ; x<destDelimiters.x_delimiter.end;x++){
        for (int y=destDelimiters.y_delimiter.begin ; y<destDelimiters.y_delimiter.end;y++){
            for (int z=destDelimiters.z_delimiter.begin ; z<destDelimiters.z_delimiter.end;z++){
                matrix->at(x).at(y).at(z).at(destDelimiters.field_component) += sourceCube[k][j][i];
                i++;
            }
            i=0;
            j++;
        }   
        i=0;
        j=0;
        k++;
    }
}

void subtract_from_4dMatrix(Matrix4D* matrix, Matrix3D sourceCube, Shape shape, Delimiter4D destDelimiters){
    int i=0;
    int j=0;
    int k=0;

    for (int x=destDelimiters.x_delimiter.begin ; x<destDelimiters.x_delimiter.end;x++){
        for (int y=destDelimiters.y_delimiter.begin ; y<destDelimiters.y_delimiter.end;y++){
            for (int z=destDelimiters.z_delimiter.begin ; z<destDelimiters.z_delimiter.end;z++){
                matrix->at(x).at(y).at(z).at(destDelimiters.field_component) -= sourceCube[k][j][i];
                i++;
            }
            i=0;
            j++;
        }   
        i=0;
        j=0;
        k++;
    }
}

// Base class for Vector fields
class Field {
public:
    Matrix4D data;
    Shape shape;
    std::vector<Delimiter4D> delimiters;
    virtual Matrix4D curl() = 0;
};

// Child class - inherits from Field
class E: public Field{
public:
    E(){}
    E(Shape s){
        shape = s;
        Matrix4D d(s.z,Matrix3D(s.y,Matrix2D(s.x,Vector1D(s.f))));
        data = d;
        // bounds to use when slicing data to calculate curl
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(1, shape.y), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y-1), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(1, shape.z), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z-1), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(1, shape.z), 0));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z-1), 0));
        delimiters.push_back(Delimiter4D(Delimiter(1, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x-1), Delimiter(0, shape.y), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(1, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x-1), Delimiter(0, shape.y), Delimiter(0, shape.z), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(1, shape.y), Delimiter(0, shape.z), 0));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y-1), Delimiter(0, shape.z), 0));

    }

    Matrix4D curl(){
        Matrix4D curl_E = Matrix4D(shape.z,Matrix3D(shape.y,Matrix2D(shape.x,Vector1D(shape.f))));

        std::vector<Matrix3D> cubes = {};
        for(auto limits: delimiters){
            Matrix3D cube = cut_out_cube(data, limits);
            cubes.push_back(cube);
        }
        // couche 0
        subtract3D(&cubes.at(0), &cubes.at(1));
        printCube(cubes.at(0));
        add_to_4dMatrix(&curl_E, cubes.at(0), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y-1), Delimiter(0, shape.z), 0));
        print4D(curl_E);
        
        subtract3D(&cubes.at(2), &cubes.at(3));
        subtract_from_4dMatrix(&curl_E, cubes.at(2), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z-1), 0));
        
        // couche 1
        subtract3D(&cubes.at(4), &cubes.at(5));
        add_to_4dMatrix(&curl_E, cubes.at(4), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z-1), 1));
        
        subtract3D(&cubes.at(6), &cubes.at(7));
        subtract_from_4dMatrix(&curl_E, cubes.at(6), shape, Delimiter4D(Delimiter(0, shape.x-1), Delimiter(0, shape.y), Delimiter(0, shape.z), 1));
        
        // couche 2
        subtract3D(&cubes.at(8), &cubes.at(9));
        add_to_4dMatrix(&curl_E, cubes.at(8), shape, Delimiter4D(Delimiter(0, shape.x-1), Delimiter(0, shape.y), Delimiter(0, shape.z), 2));
        
        subtract3D(&cubes.at(10), &cubes.at(11));
        subtract_from_4dMatrix(&curl_E, cubes.at(10), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y-1), Delimiter(0, shape.z), 2));
        return curl_E;
    }
};

// Child class - inherits from Field
class H: public Field{
public:
    H(){}
    H(Shape s){
        shape=s;
        Matrix4D d(s.z,Matrix3D(s.y,Matrix2D(s.x,Vector1D(s.f))));
        data = d;
        // bounds to use when slicing data to calculate curl
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(1, shape.y), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y-1), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(1, shape.z), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z-1), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(1, shape.z), 0));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z-1), 0));
        delimiters.push_back(Delimiter4D(Delimiter(1, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x-1), Delimiter(0, shape.y), Delimiter(0, shape.z), 2));
        delimiters.push_back(Delimiter4D(Delimiter(1, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x-1), Delimiter(0, shape.y), Delimiter(0, shape.z), 1));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(1, shape.y), Delimiter(0, shape.z), 0));
        delimiters.push_back(Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y-1), Delimiter(0, shape.z), 0));

    }

    Matrix4D curl(){
        Matrix4D curl_H = Matrix4D(shape.z,Matrix3D(shape.y,Matrix2D(shape.x,Vector1D(shape.f))));

        std::vector<Matrix3D> cubes = {};
        for(auto limits: delimiters){
            Matrix3D cube = cut_out_cube(data, limits);
            cubes.push_back(cube);
        }

        // couche 0
        subtract3D(&cubes.at(0), &cubes.at(1));
        add_to_4dMatrix(&curl_H, cubes.at(0), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(1, shape.y), Delimiter(0, shape.z), 0));
        
        subtract3D(&cubes.at(2), &cubes.at(3));
        subtract_from_4dMatrix(&curl_H, cubes.at(2), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(1, shape.z), 0));

        // couche 1
        subtract3D(&cubes.at(4), &cubes.at(5));
        add_to_4dMatrix(&curl_H, cubes.at(4), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(0, shape.y), Delimiter(1, shape.z), 1));
        
        subtract3D(&cubes.at(6), &cubes.at(7));
        subtract_from_4dMatrix(&curl_H, cubes.at(6), shape, Delimiter4D(Delimiter(1, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z), 1));
        
        // couche 2
        subtract3D(&cubes.at(8), &cubes.at(9));
        add_to_4dMatrix(&curl_H, cubes.at(8), shape, Delimiter4D(Delimiter(1, shape.x), Delimiter(0, shape.y), Delimiter(0, shape.z), 2));
        
        subtract3D(&cubes.at(10), &cubes.at(11));
        subtract_from_4dMatrix(&curl_H, cubes.at(10), shape, Delimiter4D(Delimiter(0, shape.x), Delimiter(1, shape.y), Delimiter(0, shape.z), 2));
        print4D(curl_H);
        return curl_H;
    }
};

class WaveEquation
{
private:
    Shape shape_;
    E E_;
    H H_;
    int courant_number_;
    int n_;
    int index_;

    Position source_pos(){
        Position t{ (int)floor(n_/3), (int)floor(n_/3), (int)floor(n_/2), 0 };
        return t;
    }

    float source_val(){
        return 0.1*sin(0.1*index_);
    }

public:
    WaveEquation(int n, int courant_number){
        Shape s = {
            n,
            n,
            n,
            3,
        };
        n_ = n;
        shape_ = s;
        E_ = E(s);
        H_ = H(s);
        courant_number_ = courant_number;
        index_ = 0;
    }

    void operator()(int field_component, int slice, int slice_index)
    {   
        Matrix4D field;
        Matrix3D output_cube;
        if (field_component < 3) {
            field = E_.data;
        }else{
            field = H_.data;
            field_component = field_component % 3;
        }

        if (slice == 0){
            output_cube  = cut_out_cube(field, Delimiter4D(Delimiter(slice_index), Delimiter(0, shape_.y), Delimiter(0, shape_.z), field_component));
        }else if (slice == 1){
            output_cube  = cut_out_cube(field, Delimiter4D(Delimiter(0, shape_.x), Delimiter(slice_index), Delimiter(0, shape_.z), field_component));
        }else if (slice == 2){
            output_cube  = cut_out_cube(field, Delimiter4D(Delimiter(0, shape_.x), Delimiter(0, shape_.y), Delimiter(slice_index), field_component));
        }

        Position source_pos = this->source_pos();
        float source_val = this->source_val();
    
        // std::cout << std::get<0>(source_pos) << " " << std::get<1>(source_pos) << " "<< std::get<2>(source_pos) << " "<< std::get<3>(source_pos) << " "<< std::endl;
        // std::cout<<source_val<<std::endl;

        timestep(source_pos, source_val);

        index_++;
    }

    void timestep(Position position, float val){
        // E += courant_number * curl_H(H)
        std::cout<<"gonna run curl H "<<std::endl;
        Matrix4D curl_H = H_.curl();
        // print4D(curl_H);
        std::cout<<"curl H ok"<<std::endl;
        multiply(&curl_H, courant_number_);
        std::cout<<"multiply curl H ok"<<std::endl;
        add(&E_.data, curl_H);
        std::cout<<"add curl H to E ok"<<std::endl;

        // E[source_pos] += source_val
        int x=std::get<0>(position);
        int y=std::get<1>(position);
        int z=std::get<2>(position);
        int couche=std::get<3>(position);
        E_.data.at(couche).at(x).at(y).at(z) += val;

        // H -= courant_number * curl_E(E)
        Matrix4D curl_E = E_.curl();
        multiply(&curl_E, courant_number_);
        subtract(&H_.data, curl_E);
    }

    void print_E(){
        print4D(E_.data);
    }

    void print_H(){
        print4D(H_.data);
    }

};

// int main(int argc, char** argv)
// {
//     int n = 100;
//     float courant_number = 0.1;
    
//     // unused
//     // int r = 0.01;
//     // int l = 30;

//     WaveEquation w = WaveEquation(n, courant_number);
//     int counter = 0;
//     while(counter<100){
//         w(0, 2, 50);
//         counter++;
//     }
// }

// test cut_out_cube
// int main(int argc, char** argv)
// {
//     Shape s = {
//         2,
//         2,
//         2,
//         3,
//     };

//     // my 4D Matrix
//     E e = E(s);
//     e.data[0][0][0][0] = 1.0;
//     e.data[0][1][0][0] = 2.0;
//     e.data[1][0][0][0] = 3.0;
//     e.data[1][1][0][0] = 4.0;
//     e.data[0][0][1][0] = 5.0;
//     e.data[0][1][1][0] = 6.0;
//     e.data[1][0][1][0] = 7.0;
//     e.data[1][1][1][0] = 8.0;
//     e.data[0][0][0][1] = 9.0;
//     e.data[0][1][0][1] = 10.0;
//     e.data[1][0][0][1] = 11.0;
//     e.data[1][1][0][1] = 12.0;
//     e.data[0][0][1][1] = 13.0;
//     e.data[0][1][1][1] = 14.0;
//     e.data[1][0][1][1] = 15.0;
//     e.data[1][1][1][1] = 16.0;

//     Delimiter4D delimiters = Delimiter4D(Delimiter(0, s.x), Delimiter(0,s.y), Delimiter(0, s.z), 0);
//     Matrix3D cube = cut_out_cube(e.data, e.shape, delimiters);

//     for (auto slice: cube){
//         for (auto row: slice){
//             for (auto val: row){
//                 std::cout << val << ' '<<std::endl;
//             }
//         }
//     }
//     //                            |-------|
//     //               z   |-------|| 13 14 |         
//     //           |------|| 9  10 || 15 16 |
//     //   |---y--| 5   6 || 11 12 ||-------|
//     //   x 1  2 | 7   8 ||-------|
//     //   | 3  4 ||------|
//     //   |------|

// }

// Test subtract
// int main(int argc, char** argv)
// {
//     Shape s = {
//         2,
//         2,
//         2,
//         3,
//     };

//     Matrix3D cube1 = Matrix3D(2, Matrix2D(2, Vector1D(2)));
//     cube1[0][0][0] = 1.0;
//     cube1[0][1][0] = 2.0;
//     cube1[1][0][0] = 3.0;
//     cube1[1][1][0] = 4.0;
//     cube1[0][0][1] = 5.0;
//     cube1[0][1][1] = 6.0;
//     cube1[1][0][1] = 7.0;
//     cube1[1][1][1] = 8.0;

//     Matrix3D cube2 = Matrix3D(2, Matrix2D(2, Vector1D(2)));
//     cube2[0][0][0] = 1.0;
//     cube2[0][1][0] = 2.0;
//     cube2[1][0][0] = 3.0;
//     cube2[1][1][0] = 4.0;
//     cube2[0][0][1] = 5.0;
//     cube2[0][1][1] = 6.0;
//     cube2[1][0][1] = 7.0;
//     cube2[1][1][1] = 8.0;

//     subtract(&cube1, &cube2);

//     printCube(cube1);
// }

// int main(int argc, char** argv)
// {
//     Shape s = {
//         2,
//         2,
//         2,
//         2,
//     };

//     Matrix3D cube1 = Matrix3D(2, Matrix2D(2, Vector1D(2)));
//     cube1[0][0][0] = 1.0;
//     cube1[0][1][0] = 2.0;
//     cube1[1][0][0] = 3.0;
//     cube1[1][1][0] = 4.0;
//     cube1[0][0][1] = 5.0;
//     cube1[0][1][1] = 6.0;
//     cube1[1][0][1] = 7.0;
//     cube1[1][1][1] = 8.0;

//     // my 4D Matrix
//     E e = E(s);
//     e.data[0][0][0][0] = 0.0;
//     e.data[0][1][0][0] = 0.0;
//     e.data[1][0][0][0] = 0.0;
//     e.data[1][1][0][0] = 0.0;
//     e.data[0][0][1][0] = 0.0;
//     e.data[0][1][1][0] = 0.0;
//     e.data[1][0][1][0] = 0.0;
//     e.data[1][1][1][0] = 0.0;
//     e.data[0][0][0][1] = 0.0;
//     e.data[0][1][0][1] = 0.0;
//     e.data[1][0][0][1] = 0.0;
//     e.data[1][1][0][1] = 0.0;
//     e.data[0][0][1][1] = 0.0;
//     e.data[0][1][1][1] = 0.0;
//     e.data[1][0][1][1] = 0.0;
//     e.data[1][1][1][1] = 0.0;

//     add_to_4dMatrix(&e.data, cube1, s, Delimiter4D(Delimiter(0, s.x), Delimiter(0,s.y-1), Delimiter(0, s.z), 0));
//     print4D(e.data);
// }

// Test multiply
// int main(int argc, char** argv)
// {
//     Shape s = {
//         2,
//         2,
//         2,
//         2,
//     };

//     // my 4D Matrix
//     E e = E(s);
//     e.data[0][0][0][0] = 1.0;
//     e.data[0][1][0][0] = 2.0;
//     e.data[1][0][0][0] = 3.0;
//     e.data[1][1][0][0] = 4.0;
//     e.data[0][0][1][0] = 5.0;
//     e.data[0][1][1][0] = 6.0;
//     e.data[1][0][1][0] = 7.0;
//     e.data[1][1][1][0] = 8.0;
//     e.data[0][0][0][1] = 9.0;
//     e.data[0][1][0][1] = 10.0;
//     e.data[1][0][0][1] = 11.0;
//     e.data[1][1][0][1] = 12.0;
//     e.data[0][0][1][1] = 13.0;
//     e.data[0][1][1][1] = 14.0;
//     e.data[1][0][1][1] = 15.0;
//     e.data[1][1][1][1] = 16.0;

//     multiply(&e.data, 2);

//     print4D(e.data);
// }

// Test curl_E
int main(int argc, char** argv)
{
    Shape s = {
        2,
        2,
        2,
        3,
    };

    // my 4D Matrix
    E e = E(s);
    e.data[0][0][0][0] = 1.0;
    e.data[0][1][0][0] = 2.0;
    e.data[1][0][0][0] = 3.0;
    e.data[1][1][0][0] = 4.0;
    e.data[0][0][1][0] = 5.0;
    e.data[0][1][1][0] = 6.0;
    e.data[1][0][1][0] = 7.0;
    e.data[1][1][1][0] = 8.0;
    e.data[0][0][0][1] = 9.0;
    e.data[0][1][0][1] = 10.0;
    e.data[1][0][0][1] = 11.0;
    e.data[1][1][0][1] = 12.0;
    e.data[0][0][1][1] = 13.0;
    e.data[0][1][1][1] = 14.0;
    e.data[1][0][1][1] = 15.0;
    e.data[1][1][1][1] = 16.0;
    e.data[0][0][0][2] = 17.0;
    e.data[0][1][0][2] = 18.0;
    e.data[1][0][0][2] = 19.0;
    e.data[1][1][0][2] = 20.0;
    e.data[0][0][1][2] = 21.0;
    e.data[0][1][1][2] = 22.0;
    e.data[1][0][1][2] = 23.0;
    e.data[1][1][1][2] = 24.0;

    // print4D(e.data);
    Matrix4D curl_E = e.curl();
    print4D(curl_E);
}