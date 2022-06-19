#include <math.h>
#include <vector>

float sig(float val){
    return 1/(1+pow(2.71828,-1*val));
}

class Network{
private:
    std::vector <float> input;
    std::vector <float> hidden1;
    std::vector <float> hidden2;
    std::vector <float> output;

    //weight matrices
    std::vector <std::vector <float>> wMatrix_in_h1;
    std::vector <std::vector <float>> wMatrix_h1_h2;
    std::vector <std::vector <float>> wMatrix_h2_out;

public:

    Network(){

    }

    Network(std::string path){

    }

    Network(int input, int h1, int h2, int output){

        this->input.reserve(input);
        this->hidden1.reserve(h1);
        this->hidden2.reserve(h2);
        this->input.reserve(input);
        

    }

    std::vector <float> eval(std::vector <float> input){
        return {};
    }

};