/*
Vedant V Yelsangikar
CS531 HW3
G01379948
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include <stdbool.h>
struct address_t
{
    int octet[4];
    char alias[11];
    struct address_t *leftChild, *rightChild, *parent;
    int	height,	depth;
};
struct address_t *head = NULL;
int userInput();
void inorderTraves(struct address_t *node, int n, FILE *fp)
{
    struct address_t *p = node;
    char s[20], temp[30];
    if(p->leftChild != NULL){
        inorderTraves(p->leftChild, n, fp);
    }
    snprintf(temp, 30, "%d.%d.%d.%d %s\n", p->octet[0], p->octet[1], p->octet[2], p->octet[3], p->alias);
    fprintf(fp, "%s", temp);
    memset(temp,0,sizeof(temp));
    if(p->rightChild != NULL){
        inorderTraves(p->rightChild, n, fp);
    }
}


int inorderLook(struct address_t *node, int n, char* a){
    struct address_t *p = node;
    if(n==1){
        return n;
    }
    if(p->leftChild != NULL){
        n = inorderLook(p->leftChild, n, a);
    }
    if(strcmp(a, p->alias) == 0){
        printf("\n%s: %d.%d.%d.%d\n\n", a, p->octet[0], p->octet[1], p->octet[2], p->octet[3]);
        n = 1;
        return n;
    }
    if(p->rightChild != NULL){
        n = inorderLook(p->rightChild, n, a);
    }
    return n;
}


int lookUpAddress(){    //function to find address based on entered alias
    int n=0;
    char a[11];
    struct address_t *p = head;
    printf("Enter alias: ");
    scanf("%s", a);
    n = inorderLook(head, n, a);
    if(n==0){
        printf("error: %s does not exist\n", a);
    }
    userInput();
    return 0;
}


int saveFile(){
    int n = 7;
    char s[20], temp[30];
    struct address_t *p = head;
    printf("Enter file name: ");
    scanf("%s", s);
    FILE *fp = fopen(s, "w");
    if (fp == NULL)
    {
        printf("error: %s opening the file", s);
        return -1;
    }
    inorderTraves(head, n, fp);
    fclose(fp);
    printf("File saved.\n");
    userInput();
    return 0;
}


int locationCheck(struct address_t *node, int l1, int l2, int n){     
    struct address_t *p = node;
    int i=0;
    if(p==NULL){
        printf("No elements in the list..\n");
        userInput();
        return 0;
    }
    if(p->leftChild != NULL){
        n = locationCheck(p->leftChild, l1, l2, n);
    }
    for(i=0;i<3;i++){
        if(l1==p->octet[i] && l2==p->octet[i+1]){
            n = 1;
            printf("%s\n", p->alias);
        }
    }
    if(p->rightChild != NULL){
        n = locationCheck(p->rightChild, l1, l2, n);
    }
    return n;
}


int locationValidate(int l){
    if((l >= 0 && l <= 255)){
        return 0;
    }
    printf("error: %d is an illegal entry - please reenter: \n", l);
    return 1;
}


int diplayAlias(struct address_t *node){
    int n=0, i, j=0, l = 4, l1=0, l2=0, num = 1, n1=0, n2=0;
    bool b = false;
    char c[8];
    struct address_t *p = node;
    printf("Enter Locality: ");
    scanf("%s", c);
    for(i=0;i<strlen(c);i++){
        if(c[i] == '.'){
            j++;
        }
        else if(isdigit(c[i])){
            if(j==0){
                l1 = l1 * 10;
                num = c[i] - '0';
                l1 = l1 + num;
            }
            else{
                l2 = l2 * 10;
                num = c[i] - '0';
                l2 = l2 + num;
            }
        }
        else{
            printf("error: %s is an illegal entry - please reenter: \n", c);
            diplayAlias(head);
        }
    }
    num = locationValidate(l1);
    if(num){
        diplayAlias(head);
    }
    num = locationValidate(l2);
    if(num){
        diplayAlias(head);
    }
    printf("Location: %s\n", c);
    n = locationCheck(head, l1, l2, n);
    if(n==0){
        printf("There are no values found with given locality: %s\n", c);
    }
    userInput();
    return 0;
}


void  displayList(struct address_t *node){                  //Function to display all the ip addresses with their alias, height, depth and the parent node in BST.
    struct address_t *p = node;                 
    if(p==NULL){
        printf("No elements in the list..\n");
        userInput();
        return;
    }
    if(p->leftChild != NULL){
        displayList(p->leftChild);
    }
    printf("\n%s %d.%d.%d.%d height:%d depth:%d parent:%s\n", p->alias, p->octet[0], p->octet[1], p->octet[2], p->octet[3], p->height, p->depth, p->alias); 
    if(p->rightChild != NULL){
        displayList(p->rightChild);
    }
}


void setHeight(struct address_t *node){
    struct address_t *pp = node;
    pp->height = 0;
    if(pp->leftChild != NULL){
        setHeight(pp->leftChild);
    }
    if(pp->rightChild != NULL){
        setHeight(pp->rightChild);
    }
    if((pp->leftChild == NULL) && (pp->rightChild == NULL)){
        pp->height = 0;
    }else if((pp->leftChild != NULL) && (pp->rightChild == NULL)){
        pp->height = pp->leftChild->height + 1;
    }else if((pp->leftChild == NULL) && (pp->rightChild != NULL)){
        pp->height = pp->rightChild->height + 1;
    }else{
        if(pp->leftChild->height > pp->rightChild->height)
            pp->height = pp->leftChild->height + 1;
        else
            pp->height = pp->rightChild->height + 1;
    }
}


int setDepth(struct address_t *node, int dp){
    struct address_t *p = node;
    p->depth = dp;
    dp = dp+1;
    if(p->leftChild != NULL)
        setDepth(p->leftChild, dp);
    if(p->rightChild != NULL)
        setDepth(p->rightChild, dp);
    return 0;
}
int aliasValidate(struct address_t *node, char s[], int n){         //function to validate alias
    struct address_t *pp = node;
    if(n==1){
        return n;
    }
    if(pp->leftChild != NULL){
        n = aliasValidate(pp->leftChild, s, n);
    }
    if(strcmp(s, pp->alias) == 0){
        n = 1;
        return n;
    }
    if(pp->rightChild != NULL){
        n = aliasValidate(pp->rightChild, s, n);
    }
    return n;
}
void addAddressInorder(struct address_t *root, struct address_t *temp)
{
    if(strcmp(root->alias, temp->alias) > 0)
    {
        if(root->leftChild == NULL)
        {
            temp->parent = root;
            root->leftChild = temp;
        }
        else
        {
            addAddressInorder(root->leftChild, temp);
        }
    }   
    else if(strcmp(root->alias, temp->alias) < 0)
    {
        if(root->rightChild == NULL)
        {
            temp->parent = root;
            root->rightChild = temp;
        }
        else
        {
            addAddressInorder(root->rightChild, temp);
        }
    }
    else
    {
        printf("ERROR: Entered value already exists in the BST\n");
    }
}

int addAddress(){                   //function to add address to the BST
    char s[11], a[11], ip[16];
    int al=0, o[4], i, n, j = 0, r, dc = 0, l = 4, c=0;
    printf("Enter IPv4 address: ");
    scanf("%s", ip);
    o[j] = 0;
    for(i=0; i<strlen(ip); i++){
        if(ip[i] == '.'){
            dc++;
            j++;
            o[j] = 0;
        }
        else if(isdigit(ip[i])){
            o[j] = o[j] * 10;
            n = ip[i] - '0';
            o[j] = o[j] + n;
        }
        else {
            printf("error: %s is an illegal address - please reenter: \n", ip);
            addAddress();
            return 0;
        }
    }
    if(dc!=3){
        printf("error: %s is an illegal address - please reenter: \n", ip);
        addAddress();
        return 0;
    }
    for(i=0; i<l; i++){
        if(o[i]< 0 || o[i]> 255){
            printf("error: %s ia an illegal address - please reenter: \n", ip);
            addAddress();
            return 0;
        }
    }
    while(c<=0){
        printf("Enter alias: ");
        scanf("%s", s);
        if(strlen(s) > 10){
            printf("error: %s alias name exceeds length 10\n", s);
            continue;
        }
        c++;
    }
    al = aliasValidate(head, s, al);
    if(al==1){
        printf("error: %s already exists\n", s);
        userInput();
        return 0;
    }
    struct  address_t *temp =NULL, *root = NULL;
    temp = malloc(sizeof(*temp)); 
    temp->octet[0] =  o[0];
    temp->octet[1] = o[1];
    temp->octet[2] = o[2];
    temp->octet[3] = o[3];
    strcpy(temp->alias, s);

    root = head;
    addAddressInorder(root, temp);
    userInput();
    return 0;
}


int ipAddressValidate(struct address_t *v, int l1, int l2, int l3, int l4, int n){      //
    struct address_t *pp = v;
    if(n==1){
        return n;
    }
    if(pp->leftChild != NULL){
        n = ipAddressValidate(pp->leftChild, l1, l2, l3, l4, n);
    }
    if((pp->octet[0] == l1) && (pp->octet[1] == l2) && (pp->octet[2] == l3) && (pp->octet[3] == l4)){
        printf("error: %d.%d.%d.%d already exist\n", l1, l2, l3, l4);
        n = 1;
        return n;
    }
    if(pp->rightChild != NULL){
        n = ipAddressValidate(pp->rightChild, l1, l2, l3, l4, n);
    }
    return n;
}


int ipAddressUpdate(struct address_t *node){            //function to update the address
    int l1, l2, l3, l4, num = 1, n = 0;
    while(num>0){
        printf("Enter location value #1 (0-255): ");
        scanf("%d", &l1);
        num = locationValidate(l1);
    }
    num = 1;
    while(num>0){
        printf("Enter location value #2 (0-255): ");
        scanf("%d", &l2);
        num = locationValidate(l2);
    }
    num = 1;
    while(num>0){
        printf("Enter location value #3 (0-255): ");
        scanf("%d", &l3);
        num = locationValidate(l3);
    }
    num = 1;
    while(num>0){
        printf("Enter location value #4 (0-255): ");
        scanf("%d", &l4);
        num = locationValidate(l4);
    }
    n = ipAddressValidate(node, l1, l2, l3, l4, n);
    if(n==0){
        node->octet[0] = l1;
        node->octet[1] = l2;
        node->octet[2] = l3;
        node->octet[3] = l4;
    }
    return 0;
}


int aliasInorder(struct address_t *node, char *a, int n){
    struct address_t *p = node;
    char al[11];
    if(p==NULL){
        printf("No elements in the list..\n");
        return 0;
    }
    if(n==1){
        return n;
    }
    if(p->leftChild != NULL){
        n = aliasInorder(p->leftChild, a, n);
    }
    strcpy(al, a);
    if(strcmp(p->alias, al) == 0){
        printf("Update address for %s: %d.%d.%d.%d\n", a, p->octet[0], p->octet[1], p->octet[2], p->octet[3]);
        ipAddressUpdate(p);
        n=1;
        return n;
    }
    if(p->rightChild != NULL){
        n = aliasInorder(p->rightChild, a, n);
    }
    return n;
}


int addressUpdate(){
    int n=0;
    char a[11];
    printf("Enter alias: ");
    scanf("%s", a);
    n = aliasInorder(head, a, n);
    if(n==0){
        printf("error: %s does not exist\n", a);
    }
    userInput();
    return 0;
}


struct address_t* minFind(struct address_t *node, char *min, struct address_t *n){
    struct address_t *temp=NULL;
    if(node->leftChild == NULL)
        return node;
    temp = node->leftChild;
    while(temp != NULL){
        if(strcmp(temp->alias, min) < 0){
            min = temp->alias;
            n = temp;
        }
        temp = node->leftChild;
    }
    return n;
}

//This is function to find alias in the BS tree
struct address_t* aliasFind(struct address_t *node, char s[], struct address_t *n){
    struct address_t *pp = node;
    if(n!=NULL){
        return n;
    }
    if(pp->leftChild != NULL){
        n = aliasFind(pp->leftChild, s, n);
    }
    if(strcmp(s, pp->alias) == 0){
        n = pp;
        return n;
    }
    if(pp->rightChild != NULL){
        n = aliasFind(pp->rightChild, s, n);
    }
    return n;
}


int ipAddressDelete(){                              //function to delete node using alias. Then arrange it in in-order fashion
    struct address_t *p = head, *pre = NULL, *d = NULL;
    struct address_t *temp = NULL, *lt = NULL, *rt = NULL;
    char a[11], n;
    int tmp=0, i=0;
    while(tmp<=0){
        printf("Enter alias: ");
        scanf("%s", a);
        if(strlen(a) > 10){
            printf("error: %s alias name exceeds length 10\n", a);
            continue;
        }
        tmp++;
    }
    d = aliasFind(head, a, d);
    if(d==NULL){
        printf("error: %s does not exist\n", a);
        return 0;
    }
    printf("Delete %s %d.%d.%d.%d? (y/n): ", a, d->octet[0], d->octet[1], d->octet[2], d->octet[3]);
    scanf("%s", &n);
    if(n != 'y'){
        return 0;
    }
    printf("%s deleted\n", a);
    if(d == head){
        if((head->leftChild == NULL) && (head->rightChild == NULL)){
            head = NULL;
            free(pre);
            return 0;
        }else if((d->leftChild != NULL) && (d->rightChild == NULL)){
            lt = d->leftChild;
            lt->parent = NULL;
            setDepth(lt, 0);
            d->leftChild = NULL;
            head = lt;
            free(d);
            setHeight(head);
            return 0;
        }else if((d->leftChild == NULL) && (d->rightChild != NULL)){
            rt = d->rightChild;
            rt->parent = NULL;
            setDepth(rt, 0);
            d->rightChild = NULL;
            head = rt;
            free(d);
            setHeight(head);
            return 0;
        }else{
            lt = d->leftChild;
            rt = d->rightChild;
            temp = minFind(rt, rt->alias, temp);
            strcpy(d->alias, temp->alias);
            for(i=0;i<4;i++)
                d->octet[i] = temp->octet[i];
            if(temp->parent->leftChild == temp)
                temp->parent->leftChild = temp->rightChild;
            else
                temp->parent->rightChild = temp->rightChild;
            setDepth(head, 0);
            temp->parent = NULL;
            temp->rightChild = NULL;
            free(temp);
            setHeight(head);
            return 0;
        }
    }
    pre = d->parent;
    if(d->leftChild == NULL && d->rightChild == NULL){
        if(pre->leftChild == d){
            pre->leftChild = NULL;
        }else{
            pre->rightChild = NULL;
        }
        d->parent = NULL;
        setDepth(head, 0);
        free(d);
        setHeight(head);
        return 0;
    }else if(d->leftChild != NULL && d->rightChild == NULL){
        lt = d->leftChild;
        if(pre->leftChild == d){
            pre->leftChild = lt;
        }else{
            pre->rightChild = lt;
        }
        lt->parent = pre;
        d->parent = NULL;
        d->leftChild = NULL;
        setDepth(head, 0);
        free(d);
        setHeight(head);
        return 0;
    }else if(d->leftChild == NULL && d->rightChild != NULL){
        rt = d->rightChild;
        if(pre->leftChild == d){
            pre->leftChild = rt;
        }else{
            pre->rightChild = rt;
        }
        rt->parent = pre;
        d->parent = NULL;
        d->rightChild = NULL;
        setDepth(head, 0);
        free(d);
        setHeight(head);
        return 0;
    }else{
        lt = d->leftChild;
        rt = d->rightChild;
        temp = minFind(rt, rt->alias, temp);
        strcpy(d->alias, temp->alias);
        for(i=0;i<4;i++)
            d->octet[i] = temp->octet[i];
        if(temp->parent->leftChild == temp)
            temp->parent->leftChild = temp->rightChild;
        else
            temp->parent->rightChild = temp->rightChild;
        temp->parent = NULL;
        temp->rightChild = NULL;
        setDepth(head, 0);
        free(temp);
        setHeight(head);
        return 0;
    }
}


void userInputData(int n){
    switch(n){  
        case 1:
            addAddress();
            break;
        case 2:
            lookUpAddress();
            break;
        case 3:
            addressUpdate();
            break;
        case 4:
            ipAddressDelete();
            userInput();
            break;
        case 5:
            displayList(head);
            userInput();
            break;
        case 6:
            diplayAlias(head);
            break;
        case 7:
            saveFile();
            break;
        case 8:  
            printf("Good bye!\n");
            exit(0);
            break;
        default:  
            printf("Please enter options from 1 - 8 \n");
            exit(0);
    }
}


int userInput(){       //This function is used to display options to users
    int n;
    printf(" 1) Add address \n 2) Look up address \n 3) Update address \n 4) Delete address \n 5) Display list \n 6) Display aliases for location \n 7) Save to file \n 8) Quit\n");
    printf("Enter option: ");
    scanf("%d", &n);
    if((n < 1) || (n > 8)){
        printf("As option is not from the list, Please choose an option again from the list...\n");
        userInput();
        return 0;
    }
    else{
        userInputData(n);
        return 0;
    }
    
}


int main()
{
    int i, j, num, sp = 0, h = 0;
    char c[11];
    struct address_t* tempPointer = NULL;
    struct address_t* p = NULL;
    struct address_t* parent = NULL;
    struct address_t* p1 = NULL;

    FILE* file = fopen("CS531_Inet.txt", "r"); //Read the file CS531_Inet.txt store the data in BST
    if(!file){
        printf("\n Unable to open the file");
        return -1;
    }
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        j=0;
        tempPointer = (struct address_t*)malloc(sizeof(struct address_t));
        struct address_t temp;
        temp.octet[j] = 0;
        tempPointer->octet[j] = 0;
        sp=0;
        for(i=0;i<strlen(line);i++){
            if(line[i] == ' '){
                sp++;
            }
            else if(line[i] == '.'){
                j++;
                temp.octet[j] = 0;
                tempPointer->octet[j] = 0;
            }
            else if((sp > 0) && (isdigit(line[i]) || isalpha(line[i]))){
                strncat(tempPointer->alias, &line[i], 1);
            }
            else if(isdigit(line[i])){
                temp.octet[j] = temp.octet[j] * 10;
                tempPointer->octet[j] = tempPointer->octet[j] * 10;
                num = line[i] - '0';
                temp.octet[j] = temp.octet[j] + num;
                tempPointer->octet[j] = tempPointer->octet[j] + num;
            }
        }
        tempPointer->leftChild = NULL;
        tempPointer->rightChild = NULL;
        tempPointer->parent = NULL;
        tempPointer->height = 0;
        tempPointer->depth = 0;
        if(head == NULL){             
            head = tempPointer;
            continue;
        }else{
            p = head;
        }
        while(true){
            parent = p;// parent keep track of the parent node of current node.
            if(strcmp(tempPointer->alias, parent->alias) < 0){// If data is less than current's data, node will be inserted to the left of tree
                p = p->leftChild;
                parent->height = parent->height + 1;
                if(p == NULL){
                    parent->leftChild = tempPointer;
                    tempPointer->parent = parent;
                    tempPointer->depth = parent->depth + 1;
                    tempPointer->height = 0;
                    h = 0;
                    p1 = tempPointer->parent;
                    while(p1 != NULL){
                        p1->height = h + 1;
                        h = h + 1;
                        p1 = p1->parent;
                    }
                    break;
                }
            }
            else{  // If data is greater than current's data, node will be inserted to the right of tree
                p = p->rightChild;
                if(p == NULL){
                    parent->rightChild = tempPointer;
                    tempPointer->parent = parent;
                    tempPointer->depth = parent->depth + 1;
                    tempPointer->height = 0;
                    h = 0;
                    p1 = tempPointer->parent;
                    while(p1 != NULL){
                        p1->height = h + 1;
                        h = h + 1;
                        p1 = p1->parent;
                    }
                    break;
                }
            }
        }
    }
    fclose(file);
    userInput();
    return 0;
}