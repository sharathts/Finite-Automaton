#include<stdio.h>
#include<string.h>
#include<malloc.h>

# define STATES 256
# define SYMBOLS 20

int N_symbols;	/* number of input symbols */
int NFA_states;	/* number of NFA states */
char *NFA_tab[STATES][SYMBOLS];
char *mapped_states[STATES];
int pointer = 0;
int DFA_states;	/* number of DFA states */
int DFA_tab[STATES][SYMBOLS];
char state_names[STATES][STATES];
/*Declare initial and final states*/
char *init_state = "0";
char *final_state = "3";

/*print the table*/
void print_dfa_table(int tab[][SYMBOLS], int nstates, int nsymbols)
{
	int i, j, temp = 0, iter = 0, flag;
	char *unique_states[100];
    unique_states[0] = (char *)malloc(100 * sizeof(char));
    strcpy(unique_states[0], "0");
	puts("STATE TRANSITION TABLE");
	printf("     | ");
	for (i = 0; i < nsymbols; i++)
	    printf("        %c       ", 'a'+i);

	printf("\n-----+--");
	
	for (i = 0; i < nsymbols; i++)
	    printf("----------------");
	printf("\n");
	for (i = 0; i < nstates; i++)
	{
	    if ( i == (final_state[0] - '0') && i == (init_state[0] - '0'))
	        printf("  ->*%s  | ", state_names[i]);
        else if ( i == (init_state[0] - '0'))
	        printf("  ->%s  | ", state_names[i]);
        else
        {
	        if (strstr(state_names[i], final_state))
	            printf("  *%s  | ", state_names[i]);
		    else
		        printf("  %s  | ", state_names[i]);
		}
		for (j = 0; j < nsymbols; j++)
            printf("       %s       ", mapped_states[temp++]);
		printf("\n");
	}
	printf("\n");
}
/*Declare DFA table*/
void declare_nfa_table()
{
	NFA_tab[0][0] = "012";
	NFA_tab[0][1] = "13";
	NFA_tab[1][0] = "12";
	NFA_tab[1][1] = "12";
	NFA_tab[2][0] = "01";
	NFA_tab[2][1] = "12";
	NFA_tab[3][0] = "01";
	NFA_tab[3][1] = "12";
/*	NFA_tab[4][0] = "4";*/
/*	NFA_tab[4][1] = "4";*/

	NFA_states = 3;
	DFA_states = 0;
	N_symbols = 2;
}

//	string 't' is merged into 's' in an alphabetical order, for new state

void string_merge(char *s, char *t)
{
	char temp[STATES];
	char *r , *p;
    r = temp;
    p = s;
    
	while (*p && *t)
	{
		if (*p == *t)
		{
			*r++ = *p++;
			t++;
		}
		else if (*p < *t)
		{
			*r++ = *p++;
		}
		else
			*r++ = *t++;
	}
	*r = '\0';

	if (*p)
	    strcat(r, p);
	else if (*t)
	    strcat(r, t);

	strcpy(s, temp);
}
/*Finds out the next state using lazy method*/

void get_next_state(char *next_state, char *cur_states,
	char *nfa[STATES][SYMBOLS], int n_nfa, int symbol)
{
	int i;
	char temp[STATES];
	temp[0] = '\0';
	for (i = 0; i < strlen(cur_states); i++)
		string_merge(temp, nfa[cur_states[i]-'0'][symbol]);
	strcpy(next_state, temp);
	mapped_states[pointer] = (char *)malloc(100 * sizeof(char));
	strcpy(mapped_states[pointer], temp);
	pointer += 1;
}
/*assigns state index to a state*/

int get_state_index(char *state, char state_names[][STATES], int *pn)
{
	int i;

	if (!*state) return -1;	//no next state 

	for (i = 0; i < *pn; i++)
		if (!strcmp(state, state_names[i])) return i;

	strcpy(state_names[i], state);	//new state-name
	return (*pn)++;
}

int nfa_to_dfa(char *nfa[STATES][SYMBOLS], int n_nfa, int n_sym, int dfa[STATES][SYMBOLS])
{
	
	int i = 0;	
	int n = 1;	/* number of DFA states */

	char next_state[STATES];
	int j;

	strcpy(state_names[0], init_state);	/* start state */

	for (i = 0; i < n; i++)
	{	/* for each DFA state */
		for (j = 0; j < n_sym; j++)
		{	/* for each input symbol */
			get_next_state(next_state, state_names[i], nfa, n_nfa, j);
			dfa[i][j] = get_state_index(next_state, state_names, &n);
		}
	}
	return n;	/* number of DFA states */
}

void main()
{
	declare_nfa_table();
	DFA_states = nfa_to_dfa(NFA_tab, NFA_states, N_symbols, DFA_tab);
	print_dfa_table(DFA_tab, DFA_states, N_symbols);
}

