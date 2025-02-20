#include <stdio.h>
#include <string.h>

#define MAX_USER_CHARS 21
#define USER_FILE_PATH "..\\data\\users.txt"

void load_users(int uListCols, char userlist[3][uListCols][MAX_USER_CHARS]); //loads the users.txt list to memory

void login(int uListCols, char userlist[3][uListCols][MAX_USER_CHARS], int *userid);
void signup();

int main()
{
    int userid = -1; // setting the userid to logged out -1==logged out
    int user_type=-1; // 1 for donor, 2 for recipient -1==unassigned

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
                    login(uListCols, userlist, &userid);
                    printf("ID: %d\n", userid); // userid is the column number of the username in the array
                    
                    if (strcmp(userlist[2][userid],"donor")==0)
                        user_type=1;
                    else if (strcmp(userlist[2][userid],"recipient")==0)
                        user_type=2;
                    else
                        user_type=-1;
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

    do{
        if (user_type == 1) //1 means donor
            printf("Role is donor\n");
        else if (user_type == 2) //2 means recipient
            printf("Role is recipient\n");
        else
            printf("Role is unassigned\n");
        
        printf("0=Logout\n");
        scanf("%d", &choice);
        switch(choice)
        {
            case 0:
                {
                    userid = -1;
                    printf("You have been logged out.");
                    break;
                }
        }
        
    } while (userid != -1);

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
    fgets(buffer, sizeof(buffer), fuserptr); //ignores the first line of user.txt
    
    int i=0; //ignores the first column
    while (i < uListCols && fscanf(fuserptr, "%20[^,],%20[^,],%20[^\n]\n", userlist[0][i], userlist [1][i], userlist[2][i]) == 3)
    {
        i++;
    }

    fclose(fuserptr);
}

void login(int uListCols, char userlist[3][uListCols][MAX_USER_CHARS], int *userid)
{

    char inusername[MAX_USER_CHARS]="";
    char inpassword[MAX_USER_CHARS]="";

    int usernameCol=-1;

    printf("Enter the username: ");
    scanf(" %s", &inusername);

    for(int x=0; x < uListCols; x++) //find column with matching username and set the password check to that column
    {
        if(strcmp(inusername, userlist[0][x]) == 0)
        {
            usernameCol=x;
        }
    }

    printf("Enter the password: ");
    scanf(" %s", &inpassword);

    if(strcmp(inpassword, userlist[1][usernameCol]) == 0) // check if password = username password
    {
        printf("Login successful.");
        *userid = usernameCol;
    } else
    {
        printf("Login unsuccessful. Please check if the username or password is correct.");
    }
}

void signup()
{
    printf("signup"); //temporary

}