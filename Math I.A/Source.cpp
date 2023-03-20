#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

// Optimal Pairings for T2 win:
// I = 16, P = 7, F = 11, G = 6 with 29.6173%


double I = 16, P = 7, F = 11, G = 6;
double deck = I + P + F + G;

double most = 0;
char combo[2] = { '0', '0' };

double* cards[4] = { &I, &P, &F, &G };
char names[4] = { 'I', 'P', 'F', 'G' };

double baseline = 0;

double ncr(double n, double r)
{
	if (r > n)
		return 0;
	if (r == 0 || r == n)
		return 1;
	
	double fact_n = 1.0, fact_r = 1.0, fact_nr = 1.0;
	
	for (int i = 1; i <= n; ++i) {
		fact_n *= i;
	}
	for (int i = 1; i <= r; ++i) {
		fact_r *= i;
	}
	for (int i = 1; i <= (n - r); ++i) {
		fact_nr *= i;
	}

	return fact_n / (fact_nr * fact_r);
}

enum CARDS {
	ISLAND,
	PAINTER,
	FORCE,
	GRINDSTONE
};

double solve(int up, int down)
{
	*cards[up] += 1, * cards[down] -= 1;

			double pT1 = (ncr(G, 1) * ncr(P, 1) * ncr(I, 3) * ncr(F, 2)) / ncr(deck, 7); // Must have G from beginning, perfect hand scenario
	
	
	// 4^1 unique outcomes that win
	double perms = 0; int a = 3, b = 1, c = 1, d = 2; int* nums[4] = { &a,&b,&c,&d };
	
	for (int i = 0; i <= 3; i++)
	{
		*nums[i] += 1;
		perms += ((ncr(I, a) * ncr(G, b) * ncr(P, c) * ncr(F, d)) / ncr(deck, 8));
		*nums[i] -= 1;
	}
		
	// Check G
	double passG = 0, passP = 0, passFF = 0;

	for (int i = 0; i <= 8 - 7; i++)
		passG += (1 / pow(4, i)) * ncr(G, 1 + i) * ncr(deck - G, 6 - i) / ncr(deck, 7);


			double pT2 = (1-pT1) * perms * passG;


	// 4^2 unique outcomes that win
	perms = 0;
	for (int i = 0; i <= 3; i++)
	{
		*nums[i] += 1;
		for (int j = 0; j <= 3; j++)
		{
			*nums[j] += 1;
			perms += ((ncr(I, a) * ncr(G, b) * ncr(P, c) * ncr(F, d)) / ncr(deck, 8));
			*nums[j] -= 1;
		}
		*nums[i] -= 1;
	}

	// Check G
	passG = 0;

	for (int i = 0; i <= 9 - 7; i++)
		passG += (1 / pow(4, i)) * ncr(G, 1 + i) * ncr(deck - G, 6 - i) / ncr(deck, 7);

	// Check P
	passP = 0;

	for (int i = 0; i <= 9 - 7; i++)
		passP += (1 / pow(4, i)) * ncr(P, 1 + i) * ncr(deck - P, 7 - i) / ncr(deck, 8);
	
	// Check FF
	passFF = 0;

	for (int i = 0; i <= 9 - 7; i++)
		passFF += (1 / pow(4, i)) * ncr(P, 2 + i) * ncr(deck - P, 6 - i) / ncr(deck, 8);

			double pT3 = (1 - pT1) * (1 - pT2) * perms * passG * passP * passFF;

	if ((pT1 + pT2 + pT3) > most) {

		combo[0] = names[up], combo[1] = names[down];
		most = pT1 + pT2 + pT3;
	}

	*cards[up] -= 1, * cards[down] += 1;

	return (pT1 + pT2 + pT3);
}





int main()
{
	HANDLE col;
	col = GetStdHandle(STD_OUTPUT_HANDLE);

	cout << " Running with I = " << I << ", P = " << P << ", F = " << F << ", G = " << G << endl;

	baseline = solve(0, 0);

	cout << endl << " Baseline probability is " << baseline * 100 << " %" << endl;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i != j)
			{
				cout << endl << " % probability for " << names[i] << " UP and " << names[j] << " DOWN  ---  ";
				if (solve(i, j) > baseline)
					SetConsoleTextAttribute(col, 2);
				else
					SetConsoleTextAttribute(col, 4);
				cout << solve(i, j) * 100 << " %";
				SetConsoleTextAttribute(col, 15);
			}


	cout << endl << endl << "Best Pairing is " << combo[0] << "-UP and " << combo[1] << "-DOWN" << endl;

	return 0;

}
