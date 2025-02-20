#include <stdio.h>

void load_users(); //loads the users.txt list to memory

void login();
void signup();

int main()
{
    int choice=-1;

    int uListCols;
    FILE *oUsers=NULL;
    oUsers = fopen("..\\data\\users.txt", "r");

    for (char c = getc(oUsers); c != EOF; c = getc(oUsers))  //counts the number of lines in users.txt
    {
        if (c == '\n')
        {
            uListCols = uListCols + 1;
        }
    }
    char userlist[3][uListCols][21]; //sets the columns to the number of counted columns

    printf("uListCols: %d\n", uListCols); //temporary for debugging

    /////////////////////////////////////////////////////////////// menu below
    
    printf("1 - Login\n");
    printf("2 - Signup\n");
    printf("Please the number of your choice and press enter: ");
    scanf("%d", &choice);
    switch(choice)
    {
        case 1:
            {
                char inusername[21];
                char inpassword[21];

                printf("Enter the username: ");
                scanf("%s", &inusername);

                printf("Enter the password: ");
                scanf("%s", &inpassword);

                login(inusername, inpassword);
                break;
            }
        case 2:
            {
                signup();
                break;
            }
        default:
            {
                printf("Invalid choice. Please try again.");
            }
    }
    
    /////////////////////////////////////////////////////////

    return 0;
}

void load_users()
{
    
}

void login(char inusername[21], char inpassword[21])
{
    printf("login"); //temporary
    
}

void signup()
{
    printf("signup"); //temporary

}