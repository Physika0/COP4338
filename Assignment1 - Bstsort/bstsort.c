/*
 * Name: 
 * PID: 
 *
 * I affirm that I wrote this program myself without any help
 * from or sources on the internet
 *
 * Description: This program uses a binary search tree to store
 * strings of text either inserted by user input or taken in from
 * a file. With this, it prints the strings in lexicographical
 * order either to the screen or to an output file specified by
 * the user.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct BstNode
{
	struct BstNode* left;
	struct BstNode* right;
	int counter;
	char* string;
}BstNode;

BstNode *readStdin(BstNode *root, int cflag);
BstNode *readFile(BstNode *root, char **infileName, int cflag);
BstNode *insert(BstNode *node, char *string, int cflag);
BstNode *create(char *stringKey);

void printStdout(BstNode *node);
void printToFile(BstNode *node, char *outfileName);
void freeTree(BstNode *node);

int stringcmpCaseSensitive(char *string, char *string2);
int stringcmp(char *string, char *string2);

char *copy(char *string);

int
main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
	int cflag=0, oflag=0, inflag=0;
	char **infileName = NULL;
	char *outfileName = NULL;
	static char usage[] = "usage: bstsort [-c] [-o output_file_name] [input_file_name]\n";
	
	struct BstNode *root = NULL;

	while ((c = getopt(argc, argv, "co:h")) != -1)
		switch (c) {
		case 'c':
			cflag = 1;
			break;
		case 'o':
			oflag = 1;
			outfileName = optarg;
			break;
		case 'h':
			printf("%s\n", usage);
			exit(0);
			break;
		}
	if(*(infileName = (argv + optind)))
	{
		inflag = 1;
	}
	
	if(inflag == 1)
	{

		root = readFile(root, infileName, cflag);

	}
	else
	{
		root = readStdin(root, cflag);
	}

	if(oflag == 1)
	{
		printToFile(root, outfileName);
	}
	else
	{
		printStdout(root);
	}
	
	freeTree(root);
	
	printf("\n");
	/* see what we have */
	printf("cflag = %d\n", cflag);
	printf("oflag = %d\n", oflag);
	printf("infileName = \"%s\"\n", *infileName);
	printf("outfileName = \"%s\"\n", outfileName);
	
	exit(0);
}

//Reads from stdin and inserts input to tree then root node is returned
BstNode *readStdin(BstNode *root, int cflag)
{
	char string[100];
	int stopFlag = 0;
	char *stopChar = "-1";
	while(stopFlag == 0)
	{
		printf("Enter a string of text (-1 to stop): ");
		scanf(" %[^\n]s", string);

		if((stringcmp(string, stopChar)) == 0)
		{
			stopFlag = 1;
		}
		else
		{
			root = insert(root, string, cflag);
		}
	}

	return root;
}

/*Reads input from file using a new line as a delimiter, inserts
 * the contents into the tree and returns 
 */
BstNode *readFile(BstNode *root, char **infileName, int cflag)
{
	char string[100];
	FILE *readFile;
	
	if((readFile = fopen(*infileName, "r")) == NULL)
	{
		printf("Problem opening file: %s\n", *infileName);
		exit(1);
	}

	while(fscanf(readFile, " %[^\n]s", string) != EOF)
	{
		root = insert(root, string, cflag);	
	}

	if(fclose(readFile) == EOF)
	{
		printf("Error closing %s\n", readFile);
		exit(1);
	}

	return root;
}

//Prints node data recursively to stdout in 
//inorder.
void printStdout(BstNode *node)
{
	if(node == NULL)
	{
		return;
	}

	printStdout(node->left);

	printf("%d %s\n", node->counter, node->string);
	
	printStdout(node->right);

}

//Prints to specified output file recursively
//in an inorder traversal.
void printToFile(BstNode *node, char *outfileName)
{
	FILE *writeFile;
	
	if(node == NULL)
	{
		return;
	}

	printToFile(node->left, outfileName);

	if((writeFile = fopen(outfileName, "a")) == NULL)
	{
		printf("Problem opening file: %s\n", outfileName);
		exit(1);
	}
	
	fprintf(writeFile, "%d %s\n", node->counter, node->string);
	
	if(fclose(writeFile) == EOF)
	{
		printf("Error closing %s\n", outfileName);
		exit(1);
	}

	printToFile(node->right, outfileName);
}

/*Inserts nodes into the tree recursively,
 * the string comparison function  it uses is based on whether case
 * sensitive was chosen as an option or not.
 */
BstNode *insert(BstNode *node, char *string, int cflag)
{
	if(node == NULL)
	{
		return (create(string));
	}

	if(cflag == 1)
	{
		if(stringcmpCaseSensitive(string, node->string) < 0)
		{
			node->left = insert(node->left, string, cflag);
		}
		else if(stringcmpCaseSensitive(string, node->string) > 0)
		{
			node->right = insert(node->right, string, cflag);
		}
		else
		{
			node->counter = node->counter + 1;
		}
	}
	else
	{
		if(stringcmp(string, node->string) < 0)
		{
			node->left = insert(node->left, string, cflag);
		}
		else if(stringcmp(string, node->string) > 0)
		{
			node->right = insert(node->right, string, cflag);
		}
		else
		{
			node->counter = node->counter + 1;
		}
	}

	return node;
}

/*Creates a new node and copies the passed string data for the node,
 * returns the new node.
 */
BstNode *create(char *stringKey)
{
	BstNode *node = (BstNode*) malloc(sizeof(BstNode));
	if(node == NULL)
	{
		exit(0);
	}

	node->left = NULL;
	node->right = NULL;
	node->counter = 1;
	node->string = copy(stringKey);
	
	return node;
}

/*Deallocates the memory taken up by each node 
 *(and its data) in the tree recursively.
 */
void freeTree(BstNode *node)
{
	if(node == NULL)
	{
		return;
	}

	freeTree(node->left);
	freeTree(node->right);
	free(node->string);
	free(node);

}

/*
 *Copies the passed string to the new string, character
 *by character. Returns the new string.
 */
char *copy(char *string)
{
	char *newString = (char*) calloc(100, sizeof(char));

	if(newString == NULL)
	{
		exit(0);
	}
	
	int i = 0;
	while(string[i] != '\0')
	{
		newString[i] = string[i];
		i++;
	}
	newString[i] = '\0';

	return newString;
}

/*String comparison function if case sensitive option
 * is selected. Returns 1 if greater, -1 if less, and
 * 0 if equal.
 */
int stringcmpCaseSensitive(char *string, char *string2)
{
	int i = 0;

	while(string[i] != '\0' || string2[i] != '\0')
	{
		if(string[i] > string2[i])
		{
			return 1;
		}
	
		if(string[i] < string2[i])
		{
			return -1;
		}

		i++;
	}

	return 0;
}

/*Compares two strings but case insensitive.
 * Returns 1 if greater, -1 if less, and 0
 * if equal.
 */
int stringcmp(char *string, char *string2)
{
	int i = 0;

	while(string[i] != '\0' || string2[i] != '\0')
	{
		if(tolower(string[i]) > tolower(string2[i]))
		{
			return 1;
		}

		if(tolower(string[i]) < tolower(string2[i]))
		{
			return -1;
		}
		
		i++;
	}

	return 0;
}
