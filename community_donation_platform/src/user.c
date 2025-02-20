#include <stdio.h>
#include <string.h>

#define MAX_USER_CHARS 21
#define USER_FILE_PATH "..\\data\\users.txt"

void load_users(int uListCols, char userlist[3][uListCols][MAX_USER_CHARS]); //loads the users.txt list to memory

int login(char inusername[MAX_USER_CHARS], char inpassword[MAX_USER_CHARS]);
void signup();

int main()
{
    ///////////////////////////////////////////// prepping the user list

    int uListCols=0;
    FILE *fuserptr=NULL;
    fuserptr = fopen(USER_FILE_PATH, "r");

    if (fuserptr==NULL)
    {
        printf("Opening users.txt failed.");
    }

    for (char c = getc(fuserptr); c != EOF; c = getc(fuserptr))  //counts the number of lines in users.txt
    {
        if (c == '\n')
        {
            uListCols = uListCols + 1;
        }
    }
    char userlist[3][uListCols][MAX_USER_CHARS]; //sets the columns to the number of counted columns

    printf("uListCols: %d\n", uListCols); //temporary for debugging
    fclose(fuserptr);

    /////////////////////////////////////////////// load users.txt to userlist

    load_users(uListCols, userlist);

    for (int cols=0; cols<uListCols; cols++) // temp for debug
    {
        printf("Col %d: ", cols);
        for (int rows=0; rows<3; rows++)
        {
            printf("%s ", userlist[rows][cols]);
        }
        printf("\n");
    }

    /////////////////////////////////////////////////////////////// menu 
    
    int choice=-1;
    int mLoop=1; //menu loop

    printf("1 - Login\n");
    printf("2 - Signup\n");
    printf("3 - Exit\n");

    do
    {
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
                    mLoop=0;
                    break;
                }
            case 2:
                {
                    signup();
                    mLoop=0;
                    break;
                }
            case 3:
                {
                    printf("Goodbye.");
                    mLoop=0;
                    break;
                }
            default:
                {
                    printf("Invalid choice. Please try again.\n");
                    mLoop=1;
                }
        }
    } while (mLoop==1);
    /////////////////////////////////////////////////////////

    return 0;
}

void load_users(int uListCols, char userlist[3][uListCols][MAX_USER_CHARS])
{
    FILE *fuserptr=NULL;
    fuserptr = fopen(USER_FILE_PATH, "r");

    if (fuserptr==NULL)
    {
        printf("Opening users.txt for load_users failed.");
    }

    char buffer[64];
    fgets(buffer, sizeof(buffer), fuserptr);
    
    int i=0; //ignores the first column
    while (i < uListCols && fscanf(fuserptr, "%20[^,],%20[^,],%20[^\n]\n", userlist[0][i], userlist [1][i], userlist[2][i]) == 3)
    {
        i++;
    }

    fclose(fuserptr);
}

int login(char inusername[21], char inpassword[21])
{
    int user_type=-1; // 1 for donor, 2 for recipient
    printf("login"); //temporary
    
    return user_type;
}

void signup()
{
    printf("signup"); //temporary

}