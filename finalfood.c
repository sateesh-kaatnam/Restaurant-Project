#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node
{
	char name[50];
	int cost;
	struct node *next1;
	struct node *next2;
};
struct node *finalhead=NULL;
char pkemail[40],city[20],rest[20];

void register1();									//New user registration
void login();										//Reads the login credentials from user
int checkLogin(char *,char *,char *);				//Checks the login credentials from the file
int sessionCheck();									//Checks for the user already logged in or not					
void sessionStart();								//creates a new session if user logeed in
struct node* getRestaurents(char *,char *,char *);	//It gets all the restaurents in a city
struct node * restaurentlist(FILE *);				//gets all the information of a restaurent in a linked list
void getItemsInRestaurent(struct node *,int);		//gets all the items available in restaurent
void selectRestaurent();							//Suggest the user to choose a restaurent from a list
void displayMenuList(struct node *);				//Displays the items available in the restaurent(MENU CARD)
void displayretaurents(struct node *);				//Displayes all the restaurents available in the city
void viewProfile();									//Displays the  previous transactions of a particular user
void userDashHome();								//Displays the user Home Page
int checkUserExist(char *);							//Checks the user is exsisting user or a new user
void placeOrder(struct node *,int *,int *,char *);		//Place the orders ordered by the user
void logout();										//Ends the user session
int main()
{
	if(sessionCheck())
	{
	FILE *fp=fopen("session.txt","r");
	char buff[50];
	fgets(buff,40,fp);
	fclose(fp);
	finalhead=getRestaurents(buff,NULL,NULL);
	int i;
	char email[40];
	for(i=0;buff[i]!='-';i++)
		email[i]=buff[i];
	email[i]='\0';
	
	strcpy(pkemail,email);
	//strcpy
	userDashHome();	
	}
	else{
	
	int ch;
	printf("--------------WELCOME TO FOOD PANDA------------------\n");
	printf("1.Login\n2.Register\n3.Exit\n");
	printf("Enter Your Choice:");
	scanf("%d",&ch);
	switch(ch)
	{
		case 1:
			login();
			break;
		case 2:
			register1();
			break;
		case 3:
			exit(0);
	}
}
	return 0;
}
int sessionCheck()
{
FILE *fp=fopen("session.txt","r");
char buff[20];
fgets(buff,40,fp);
if(buff[0]!='0')
	return 1;
else
	return	0;
}
void sessionStart()
{
	FILE *fp=fopen("session.txt","w");
	fprintf(fp,"%s-%s",pkemail,city);
	fclose(fp);
}


void logout(){										//Ends the user session
FILE *fp=fopen("session.txt","w");
fprintf(fp,"%d",0)	;
fclose(fp);
main();
}

int checkUserExist(char *email){					//CHECKS FOR USER ALREADY REGISTERED OR NOT
	FILE *fp=fopen("registers.txt","r");
	char buff[50];
	
while(fgets(buff, 255, fp)!=NULL)
	{
		//puts(buff);
		if(!strncmp(email,buff,strlen(email)-1))
		{
			printf("\nYOUR EMAIL HAS ALREADY REGISTERED\n");
			fclose(fp);
			return 0;
		}
		
	}
	fclose(fp);
	return 1;	
}

void register1()		//REGISTRATION FOR A NEW USER
{
	FILE *fp;
	char username[15],password[15],email[40],phone[13],city[30];
	fp=fopen("registers.txt","a");
	printf("Enter Name:");
	scanf("\n");
	gets(username);
	printf("Enter the valid email address:");
	gets(email);
	strlwr(email);
	printf("Enter password:");
	gets(password);
	printf("Enter the phone number:");
	gets(phone);
	printf("Enter the city:");
	gets(city);
	strlwr(city);
	if(checkUserExist(email)){
	
		fprintf(fp,"%s-%s-%s-%s-%s\n",email,password,username,phone,city);
		printf("Successfully Registered\n");
		fclose(fp);
		main();
}
	else
		main();
	fclose(fp);
}


void login() //TAKES LOGIN CREDENTIALS FROM THE USER  
{
	FILE *fp;
	
	fp=fopen("registers.txt","r");
	char username[40],password[15];
	printf("Enter the email:");
	scanf("\n");
	gets(username);
	printf("Enter the password:");
	gets(password);
char buffer[255];
int flag=0;
while(fgets(buffer, 255, fp)!=NULL)
{
		if(checkLogin(buffer,username,password)){
			flag=1;
			finalhead=getRestaurents(buffer,username,password);
			strcpy(pkemail,username);
			break;
		}
		
}
 fclose(fp);
if(flag==1){
	sessionStart();
	userDashHome();
}
else
{
	printf("\nSorry Invalid Credentials..!\n");
	main();
}
}
int checkLogin(char *buffer,char *username,char *password)	//LOGIN CREDENTIALS VALIDATION
{
	char user[40],pass[15];
	int i,k=0;
	for(i=0;buffer[i]!='-';i++)
		user[i]=buffer[i];
	user[i]='\0';
	for(i+=1;buffer[i]!='-';i++)
		pass[k++]=buffer[i];
	pass[k]='\0';
	if(!strcmp(user,username) && !strcmp(pass,password))
	{
		return 1;
	}
	return 0;
}

void userDashHome()							//Displays user home page
{
	printf("\n--------------HELLO %s WELCOME TO FOOOD PANDA---------------\n",pkemail);
 int ch;
 printf("PRESS -1 TO GET RESTAURENTS IN YOUR CITY\n");
 printf("PRESS -2 TO VIEW YOUR PROFILE\n");
 printf("PRESS -3 TO LOGOUT\n");
 scanf("%d",&ch);
 if(ch==-1){
 	selectRestaurent();
 }
 else if(ch==-2)
 	viewProfile();
else if(ch==-3)
	logout();
}

void selectRestaurent() //SELECTS A SINGLE RESTAURENT FORM LIST OF RESTAURENTS 
 {
 if(finalhead!=NULL){
 
 int rest_select;
 
	printf("----------------------------------------\nPRESS -1 TO LOGOUT\n");
	printf("\nPRESS -2 FOR HOMEPAGE\n");
	printf("RESTAURENTS IN %s\n ",city);
 	displayretaurents(finalhead);
	printf("\nSelect A Restaurent:");
	scanf("%d",&rest_select);
	if(rest_select==-1)
		logout();
	else if(rest_select==-2)
		userDashHome();
	else{
		getItemsInRestaurent(finalhead,rest_select);
		}
	}
	

}
void viewProfile()
{
	FILE *fp=fopen("orders.txt","r");
	char buff[500];
	printf("\nHELLO %s Your Previous Orders\n---------------------------------\n ",pkemail);
	while(fgets(buff, 255, fp)!=NULL)
	{
		if(!strncmp(pkemail,buff,strlen(pkemail)-1))
		{
			int i;
			
			for(i=strlen(pkemail)+1;buff[i]!='-';i++)
				printf("%c",buff[i]);
			printf("\n");
			
			for(;buff[i]!='\0';i++)
			{
				if(buff[i]=='-')
					printf("\n");
				else
					printf("%c",buff[i]);
			}
			printf("-----------------------------\n");
		}
		
	}
	int ch;
	printf("\nPRESS 1 TO LOGOUT\n");
	printf("PRESS 2 TO GET BACK\n");
	scanf("%d",&ch);
	if(ch==1)
		logout();
	else
		userDashHome();
}
void getItemsInRestaurent(struct node *head,int rest)  //TAKING ORDERS FROM THE CUSTOMER
{
	struct node *temp=head;
	int i,choice;
	char ch;
	int *orderarr=(int *)malloc(50*sizeof(int)),*quantarr=(int *)malloc(50*sizeof(int));
	for(i=1;i!=rest;i++,temp=temp->next1);
	char hotel[20];
	strcpy(hotel,temp->name);
	puts(hotel);
	i=0;int j=0;
	do{
	printf("\n--------------------------------------\n*********************MENU********************\n");
	displayMenuList(temp->next2);
	printf("\n-----------------------------------\n");
	printf("PRESS -1 TO LOGOUT\n");
	printf("PRESS -2 TO GET RESTAURENT LIST\n");
	printf("Enter the item which you want to order:");
	scanf("%d",&choice);
	printf("Enter the quantity:");
		scanf("%d",&quantarr[j++]);
	orderarr[i++]=choice;
	if(choice==-1)
		logout();
	else if(choice==-2)
		selectRestaurent();
	printf("Do you Want another Item? Y or N\n");
	scanf("\n%c",&ch);
	}while(ch!='N' && ch!='n');
	placeOrder(temp->next2,orderarr,quantarr,hotel);
}


struct node* getRestaurents(char *buffer,char *user,char *pass) 	//GETS THE ALL THE RESTAURENTS AVILABLE IN THE CITY
{
	char location[15];
	int i,k=0,count=0;
	for(i=strlen(buffer)-1;buffer[i]!='-';i--);
	for(i+=1;buffer[i]!='\0';i++)
		location[k++]=buffer[i];
	location[k]='\0';
	FILE *fp;
	char buff[255];
	struct node *head=NULL;
	fp=fopen("restaurent.txt","r");
	while(fgets(buff, 255, fp)!=NULL){								//reading the restaurents form file restaurents.txt
		count=0;
		for(i=0;location[i]!='\0';i++)
		{
				if(location[i]==buff[i])
					count++;
				else
					break;
		}
	if(count==strlen(location))
	{
		//printf("The Restaurents In %s\n",location);
		strcpy(city,location);
		head=restaurentlist(fp);
		break;
	}
}
fclose(fp);
return head;	
}
struct node * createNode(char *resname)							//Creates new nodes for the restaurents available in the city
{
	struct node* newnode=(struct node*)malloc(sizeof(struct node));
	strcpy(newnode->name,resname);
	newnode->next1=NULL;
	newnode->next2=NULL;
	return newnode;
}
struct node * createItemNode(char *resname,char *cost1)			//creates new node for items available in each city
{
	struct node* newnode=(struct node*)malloc(sizeof(struct node));
	strcpy(newnode->name,resname);
	newnode->cost=atoi(cost1);
	newnode->next1=NULL;
	newnode->next2=NULL;
	return newnode;
}
struct node* createMenuList(struct node*head,char *buff,int *i)	//CREATES THE MENU LIST FOR ITES AVAILABLE IN A RESTAURENT
{
	struct node* newnode=NULL;
	while(buff[*i]!='\0')
	{
		char itemname[20],cost1[20];
		int k=0;
		for(;buff[*i]!='-'&&buff[*i]!='\0';*i+=1)
				itemname[k++]=buff[*i];
		itemname[k]='\0';
		k=0;
		for(*i+=1;buff[*i]!='-'&&buff[*i]!='\0';*i+=1)
				cost1[k++]=buff[*i];
		cost1[k]='\0';
		newnode=createItemNode(itemname,cost1);
		if(head==NULL)
			head=newnode;
		else
		{
			struct node *temp=head;
			for(;temp->next1!=NULL;temp=temp->next1);
				temp->next1=newnode;
		}
		*i+=1;
	}
	return head;
}
struct node* createRestList(struct node *head,char *buff) //CREATES THE RESTAURENT LIST
{
	char restname[20];
	int i,k=0;
	for(i=0;buff[i]!='-';i++)
		restname[k++]=buff[i];
	restname[k]='\0';
	struct node *newnode=createNode(restname);
	if(head==NULL)
	{
		head=newnode;
		i+=1;
		head->next2=createMenuList(head->next2,buff,&i);
	}
	else
	{
		struct node *temp=head;
		for(;temp->next1!=NULL;temp=temp->next1);
		temp->next1=newnode;
		i+=1;
		temp->next2=createMenuList(temp->next2,buff,&i);
	}
	return head;
}
struct node* restaurentlist(FILE *fp)			//TAKE THE RESTAURENT LIST FROM FILE 
{
	struct node *head=NULL;
	char buff[500];
	while(fgets(buff, 500, fp)!=NULL){
		buff[strlen(buff)-1]='\0';
		if(!(strcmp(buff,"end")))
			break;
		else if(head==NULL)
			head=createRestList(head,buff);	
		else
		{
			struct node *temp=head;
			for(;temp->next1!=NULL;temp=temp->next1);
			temp->next1=createRestList(temp->next1,buff);
		}
	}
return head;
}
void displayMenuList(struct node*head)			//DISPLAY THE ITEMS AVAILABLE IN RESTAURENT
{
	struct node *temp=head;
	int i=1;
	if(temp!=NULL){
		for(;temp!=NULL;temp=temp->next1)
			printf("%d. %s - %d \n",i++,temp->name,temp->cost);
	}
}
void displayretaurents(struct node *head)		//DISPLAY THE RESTAURENTS IN THE CITY
{
	struct node *temp=head;int i;
	if(head==NULL)
		printf("yes\n");
	for(i=1;temp!=NULL;temp=temp->next1)
		printf("%d. %s\n",i++,temp->name);
}
void placeOrder(struct node *head,int *arr,int *quant,char *hotel)	//PLACE ORDERS ORDERED BY THE CUSTOMER
{
	struct node *temp=head;
	char orders[20][20]={{'\0'}};
	
	int i=0,j=0,bill_amount=0;
	printf("\n\n------------------%s YOU HAVE ORDERED------------------------ ",pkemail);
	printf("\n---------------------------------------\nItem-Quantity-Price\n-------------------------------------------\n");
	for(i=1;temp!=NULL;temp=temp->next1,i++)
	{
		for(j=0;j<=sizeof(arr)/sizeof(int)+1;j++)
		if(i==arr[j])
		{
			
			strcpy(orders[j],temp->name);
			bill_amount+=(quant[j]*temp->cost);
			printf("%s-%d-%d\n",orders[j],quant[j],temp->cost);
			
		}
	}
	free(arr);
	free(quant);
	printf("\n----------------------------------------\nTOTAL BILL :%d",bill_amount);
	char ch;
	printf("\n----------------------------------------\nARE YOU SURE TO PLACE THE ORDER? Y OR N\n");
	scanf("\n%c",&ch);
	if(ch=='Y' || ch=='y')
	{
		printf("Order Placed Sucessfully..!\n\n");
		FILE *fp=fopen("orders.txt","a");
		fprintf(fp,"%s-%s",pkemail,hotel);			//places the oreders of users in orders.txt
		for(i=0;orders[i][0]!='\0';i++)
			fprintf(fp,"-%s",orders[i]);
		fprintf(fp,"-BILL:%d\n",bill_amount);
		fclose(fp);
		userDashHome();		
	}
	else
	{
		printf("\nYour has been cancelled\n");
		printf("\nSELECT A RESTAURENT\n");
		selectRestaurent();
		}	
}
