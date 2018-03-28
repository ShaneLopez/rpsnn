#include<iostream>
#include<ctime>
#include<cmath>
#include<cstdlib>
#include<string>
#include<fstream>
using namespace std;

const double e = 2.718281828459045235;

const int inputs = 2;
const int hidden = 12;
const int outputs = 2;
int epochs = 2000;

double weightih[inputs + 1][hidden] = {0.0};
double weightho[hidden + 1][outputs] = {0.0};
double lr_ho = 0.7;
double lr_ih = 0.07;

double inputval[inputs];
double hiddenval[hidden] = {0.0};
double outputval[outputs] = {0.0};
double error[outputs] = {0.0};

double getrand();
void initweights();
void runnet();
void backpropho();
void backpropih();

int main(int argc, char ** argv)
{
	srand((unsigned)time(0));

	string opp;
	string comp;
	int temp;
	int move[2] = {0};
	int win = 0;

	ofstream fout;

	initweights();

	temp = rand() % 1000 + 1;

        if(temp % 2 == 0)
        {
       		inputval[0] = -1;
        	inputval[1] = -1;
     	}
   	else if(temp % 3 == 0)
  	{
              	inputval[0] = -1;
            	inputval[1] = 1;
    	}
    	else if(temp % 5 == 0)
  	{
             	inputval[0] = 1;
             	inputval[1] = -1;
    	}
      	else
       	{
           	inputval[0] = 1;
            	inputval[1] = -1;
   	}

	for(int i = 0; i < epochs; i++)
	{
		runnet();

		for(int j = 0; j < outputs; j++)
		{
			if(outputval[j] >= 0.9)
			{
				move[j] = 1;
			}
			else if(outputval[j] <= -0.9)
			{
				move[j] = -1;
			}
		}

		if(inputval[0] == -1 && inputval[1] == -1)
		{
			opp = "rock";
		}
		else if(inputval[0] == -1 && inputval[1] == 1)
		{
			opp = "paper";
		}
		else
		{
			opp = "scissors";
		}

		if(move[0] == -1 && move[1] == -1)
                {
                        comp = "rock";
                }
                else if(move[0] == -1 && move[1] == 1)
                {
                        comp = "paper";
                }
                else
                {
                        comp = "scissors";
                }

		cout << opp << " " << comp << endl;
		//cout << outputval[0] << " " << outputval[1] << endl;

		backpropho();
		backpropih();

		if(opp == "rock" && comp == "scissors")
        	{
                	win++;
        	}
        	else if(opp == "paper" && comp == "rock")
        	{
        	        win++;
        	}
		else if(opp == "scissors" && comp == "paper")
		{
			win++;
		}
	}

	cout << win << endl;

	fout.open("weights.txt");

	for(int i = 0; i < hidden; i++)
	{
		for(int j = 0; j < inputs + 1; j++)
		{
			fout << weightih[j][i] << " ";
		}
	}

	for(int i = 0; i < outputs; i++)
	{
		for(int j = 0; j < hidden + 1; j++)
		{
			fout << weightho[j][i] << " ";
		}
	}

	fout.close();

	return 0;
}

double getrand()
{
	return double(rand() / double(RAND_MAX));
}

void initweights()
{
	for(int i = 0; i < hidden; i++)
	{
		for(int j = 0; j < inputs + 1; j++)
		{
			weightih[j][i] = (getrand() - 0.05) / 2;
		}
	}

	for(int i = 0; i < outputs; i++)
	{
		for(int j = 0; j < hidden + 1; j++)
		{
			weightho[j][i] = (getrand() - 0.5) / 5;
		}
	}
}

void runnet()
{
	for(int i = 0; i < hidden; i++)
	{
		hiddenval[i] = 0.0;
		for(int j = 0; j < inputs; j++)
		{
			hiddenval[i] += inputval[j] * weightih[j][i];			
		}
		hiddenval[i] += weightih[inputs + 1][i];

		hiddenval[i] = tanh(hiddenval[i]);
	}

	for(int i = 0; i < outputs; i++)
	{
		outputval[i] = 0.0;
		for(int j = 0; j < hidden; j++)
		{
			outputval[i] += hiddenval[j] * weightho[j][i];
		}
		outputval[i] += weightho[hidden + 1][i];

		outputval[i] = tanh(outputval[i]);
	}


	int temp = rand() % 1000 + 1;

   	if(temp % 2 == 0)
      	{
          	inputval[0] = -1;
             	inputval[1] = -1;
   	}
     	else if(temp % 3 == 0)
     	{
              	inputval[0] = -1;
              	inputval[1] = 1;
     	}
     	else if(temp % 5 == 0)
    	{
           	inputval[0] = 1;
              	inputval[1] = -1;
      	}
     	else
    	{
           	inputval[0] = 1;
             	inputval[1] = -1;
     	}


	if(inputval[0] == -1 && inputval[1] == -1)
	{
		error[0] = outputval[0] - (-1);
		error[1] = outputval[1] - 1;
	}
	else if(inputval[0] == -1 && inputval[1] == 1)
	{
		error[0] = outputval[0] - 1;
		error[1] = outputval[1] - (-1);
	}
	else if(inputval[0] == 1 && inputval[1] == -1)
	{
		error[0] = outputval[0] - (-1);
		error[1] = outputval[1] - (-1);
	}
}

void backpropho()
{
	double weightchange;
	for(int i = 0; i < outputs; i++)
	{
		for(int j = 0; j < hidden; j++)
		{
			weightchange = lr_ho * error[i] * hiddenval[j];
			weightho[j][i] -= weightchange;

			if(weightho[j][i] < -5)
			{
				weightho[j][i] = -5;
			}
			else if(weightho[j][i] > 5)
			{
				weightho[j][i] = 5;
			}
		}
		weightho[hidden + 1][i] -= lr_ho * error[i];
	}
}

void backpropih()
{
	double x;
	double toterr = (error[0] * 0.75) + error[1];
	for(int i = 0; i < hidden; i++)
	{
		for(int j = 0; j < inputs; j++)
		{
			x = 1 - (hiddenval[i] * hiddenval[i]);
			x = x * weightih[j][i] * toterr * lr_ih;
			x = x * inputval[j];
			weightih[j][i] -= x;
		}

		x = 1 - (hiddenval[i] * hiddenval[i]);
               	x = x * weightih[inputs + 1][i] * toterr * lr_ih;
               	weightih[inputs + 1][i] -= x;
	}
}
