#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

typedef struct donation {
  int id;
  char supply[30];
  char code[5];
  char donator[20];
  int nShip;
  float qty;
  struct donation* next;//point to the next node
  struct donation *prev;//point to the previous node
} DonationN;

typedef struct distribution {
  int id_d;
  char code_d[5];
  char dest_d[20];
  float qty_d;
  struct distribution* next;
  struct distribution *prev;
} DistN;


// Function Prototype ----------------------------------
DonationN* createNode();
DonationN* addItemToList();
void printList();
DonationN *ReadLinkedList();
void writeListToDonationFile();
void modifyDonationFile();
void searchDonation();
void searchDonationTotal();
void cleanUpMemory();

DistN* createDistNode();
void printDistList();
DistN* readDistLinkedList();
void writeListToDistFile();
DistN* subtractFromDist();
DistN* addItemToDistList(DonationN* ptr, DistN* ptr_d, float distQty);
void searchDist();
void searchDistTotal();
void cleanUpDistMemory();

// Display menu ----------------------------------
void mainMenu() {
  printf("\n====================================================\n");
  printf("COVID-19 DONATION MANAGEMENT SYSTEM\n");
  printf("====================================================\n");
  printf("\n1 - Create Donation Details\n");
  printf("2 - Distribute Supply\n");
  printf("3 - Search Details\n");
  printf("4 - List All Distributed Donation & Accumulated Quantities\n");
  printf("5 - Quit\n");
  printf("\n----------------------------------------------------\n\n");
}

int searchMenu() {
  int choice;
  printf("\n1 - Search Donation Details\n");
  printf("2 - Search Distribution Details\n");
  printf("3 - Search Total Quantity of Specific Donation Supply\n");
  printf("4 - Search Total Quantity of Specific Distributed Supply\n");
  printf("5 - Back To Main Menu\n");
  printf("\n----------------------------------------------------\n\n");
  scanf("%d", &choice);

  return choice;
}

void printSupplyCode() {
  printf("\n## SUPPLY CODE ##\n");
  printf("CT - Contactless Thermometer\n");
  printf("HS - Hand Sanitizer\n");
  printf("FM - Face Mask\n");
  printf("SM - Surgical Mask\n");
  printf("OM - Oxygen Mask\n\n");
}


void makeUpper(char *str) {
  int i;
  
  for (i = 0; str[i] != '\0'; i++)
  {
      str[i] = toupper(str[i]);
  }
}


// Create new DonationN node ----------------------------------
DonationN* createNode() {
  char supply[30], code[5], donator[20];
  int id, nShip;
  float qty;

  printf("Required Information:\n");
  printf("- Name of Supply\n - Supply Code\n - Donator\n - Number of Shipment\n - Quantity Received\n\n");

  printf("Enter Name of Supply:\n");
  scanf("%s", supply);
  printf("Enter Supply Code:\n");
  scanf("%s", code);
  printf("Enter Donator:\n");
  scanf("%s", donator);
  printf("Enter Number of Shipment:\n");
  scanf("%d", &nShip);
  printf("Enter Quantity Received:\n");
  scanf("%f", &qty);

  DonationN* newNode = malloc(sizeof(DonationN));

  newNode->id = rand();
  strcpy(newNode->supply, supply);
  strcpy(newNode->code, code);
  strcpy(newNode->donator, donator);
  newNode->nShip = nShip;
  newNode->qty = qty;

  if (newNode->nShip <= 0 || newNode->qty <= 0) {
    printf("ERROR: Number of Shipment and Quantity Received must be greater than 0.\nPlease enter again.\n");
    printf("Number of Shipment: \n");
    scanf("%d", &nShip);
    printf("Quantity Received: \n");
    scanf("%f", &qty);
  }

  printf("\n--- ADDED ---\n");
  printf("ID:%d Name of Supply : %s Supply Code : %s Donator : %s Number of Shipment : %d Quantity Received : %.2f\n\n",
    newNode->id, newNode->supply, newNode->code, newNode->donator, newNode->nShip, newNode->qty);

	newNode->next = NULL;
	newNode->prev = NULL;
  
  return newNode;
}


// Add new item to Linked List ----------------------------------
DonationN* addItemToList(DonationN* ptr) {

  DonationN* newItem = createNode();

  if (ptr != NULL) {
    ptr->prev = newItem;
    newItem->next = ptr;
  }

  return newItem;
}


// Print the added item on Donation Linked List ----------------------------------
void printList(DonationN *head) {
  DonationN* temp = head;
  int count = 0;

  DonationN* pHead = NULL;
  DonationN* pLast = NULL;

  //loop until the last node bc its "next" pointer points to NULL
  while (temp != NULL) {
    count++;

    pHead = temp->next;
    pLast = temp->prev;

    printf("[%d] ID: %d Name of Supply: %s Supply Code: %s Donator: %s Number of Shipment: %d Quantity Received: %.2f\n",
      count, temp->id, temp->supply, temp->code, temp->donator, temp->nShip, temp->qty);

    temp = temp->next;
    pHead = NULL;//reset for the next execution
    pLast = NULL;//reset for the next execution
  }
}

// Print the added item on DistN Linked List ===============
void printDistList(DistN *head) {
  DistN* temp = head;
  int count = 0;

  DistN* phead = NULL;
  DistN* plast = NULL;

  //loop until the last node bc its "next" pointer points to NULL
  while (temp != NULL) {
    count++;

    phead = temp->next;
    plast = temp->prev;

    printf("[%d] ID: %d Supply Code: %s Destination: %s Quantity Received: %.2f\n",
      count, temp->id_d, temp->code_d, temp->dest_d, temp->qty_d);

    temp = temp->next;
    phead = NULL;//reset for the next execution
    plast = NULL;//reset for the next execution
  }

}

//Read all items on donation.txt to Linked List ----------------------------------
DonationN* readLinkedList(DonationN* head) {
  
  FILE* fp;
  fp = fopen("donation.txt", "r");
  int fsize, i;
  bool fileEmp = false;

  DonationN* ptr = head;
  DonationN* plast;

  fseek(fp, 0, SEEK_END);//set SEEK_END to read the file size by using ftell 
  
  if (ptr == NULL) {
    ptr = malloc(sizeof(DonationN));
    head = ptr;
  }

  fsize = (int)ftell(fp);
  
  if (fsize <= 0) {
    fileEmp = true;
    printf("File is empty.\n");
    ptr = NULL;
    head = ptr;
  }

  if (fileEmp == false) {
    rewind(fp);
    
    while (fscanf(fp, "%d %s %s %s %d %f",
      &ptr->id, ptr->supply, ptr->code, ptr->donator, &ptr->nShip, &ptr->qty)) {
      
      if (feof(fp)) {
        break;
      }
      ptr->next = malloc(sizeof(DonationN));
      ptr = ptr->next;
      plast = ptr;
      ptr->next = NULL;
    }
  }
  
  ptr = NULL;
  plast = NULL;
  
  fclose(fp);
  return head;
}

// Read all items on dist.txt to Linked List ===============
DistN* readDistLinkedList(DistN* head_d) {
  
  FILE* fp;
  fp = fopen("dist.txt", "r");
  int fsize, i, fileEmp = 0;

  DistN* ptr = head_d;
  DistN* plast;

  fseek(fp, 0, SEEK_END);
  
  if (ptr == NULL) {
    ptr = malloc(sizeof(DistN));
    head_d = ptr;
  }

  fsize = (int)ftell(fp);
  
  if (fsize <= 0) {
    fileEmp = 1;
    printf("File is empty.\n");
    ptr = NULL;
    head_d = ptr;
  }
  
  if (fileEmp == 0) {
    rewind(fp);
    
    while (fscanf(fp, "%d %s %s %f", &ptr->id_d, ptr->code_d, ptr->dest_d, &ptr->qty_d)) {
      
      if (feof(fp)) {
        break;
      }
      
      ptr->next = malloc(sizeof(DistN));
      ptr = ptr->next;
      plast = ptr;
      ptr->next = NULL;
    }
    
  }
  
  fclose(fp);
  return head_d;
}



// Write items in Linked List on donation.txt ----------------------------------
void writeListToDonationFile(DonationN* head) {

  FILE* fp;
  fp = fopen("donation.txt", "a");

  if (fp != NULL) {
    DonationN* curr = head;
    DonationN* pnext = NULL;
    DonationN* pprev = NULL;

    while (curr != NULL) {
      pnext = curr->next;
      pprev = curr->prev;

      curr->next = NULL;
      curr->prev = NULL;

      fprintf(fp, "\n");
      fprintf(fp, "%d ", curr->id);
      fprintf(fp, "%s ", curr->supply);
      fprintf(fp, "%s ", curr->code);
      fprintf(fp, "%s ", curr->donator);
      fprintf(fp, "%d ", curr->nShip);
      fprintf(fp, "%.2f", curr->qty);
      
      printf("\nWriting [%d] %s to donation.txt file.......", curr->id, curr->supply);

      curr->next = pnext;
      curr->prev = pprev;

      pnext = NULL;
      pprev = NULL;

      curr = curr->next;
    }

    fclose(fp);
    fp = NULL;
    printf("\nFile updated successfully :)\n");
  }
  else {
    printf("ERROR: FILE CAN'T OPEN FOR WRITING.\n");
  }

}

// Write items in DistN Linked List on dist.txt ===============
void writeListToDistFile(DistN* head_d) {

  FILE* fp;
  fp = fopen("dist.txt", "a");

  if (fp != NULL) {
    DistN* curr = head_d;
    DistN* pnext = NULL;
    DistN* pprev = NULL;

    while (curr != NULL) {
      pnext = curr->next;
      pprev = curr->prev;

      curr->next = NULL;
      curr->prev = NULL;
      
      fprintf(fp, "\n");
      fprintf(fp, "%d ", curr->id_d);
      fprintf(fp, "%s ", curr->code_d);
      fprintf(fp, "%s ", curr->dest_d);
      fprintf(fp, "%.2f", curr->qty_d);

      printf("\nWriting [%d] %s to dist.txt file.......", curr->id_d, curr->dest_d);

      curr->next = pnext;
      curr->prev = pprev;

      pnext = NULL;
      pprev = NULL;

      curr = curr->next;
    }

    fclose(fp);
    fp = NULL;
    printf("\nFile updated successfully :)\n");
  }
  else {
    printf("ERROR: FILE CAN'T OPEN FOR WRITING.\n");
  }
}



// distribute item ----------------------------------
DistN* subtractFromDist(DonationN* head, DistN* head_d) {

  float distQty;
  int targetId;

  DonationN* temp = head;
  DonationN* pnext;

  DistN* temp_d = head_d;

  if (temp != NULL) {
    printf("\nEnter ID of the donation supply: ");
    scanf("%d", &targetId);

    while (temp != NULL) {
      
      if (temp->id == targetId) {
        printf("ID: %d Name of Supply: %s Supply Code: %s Donator: %s Number of Shipment: %d Quantity Received: %.2f\n",
          temp->id, temp->supply, temp->code, temp->donator, temp->nShip, temp->qty);

        printf("\nEnter a number of quantity for distribution: ");
        scanf("%f", &distQty);

        if (distQty <= 0) {
          printf("ERROR: Number of Quantity Distributed must be greater than 0.\nPlease enter again.\n");
          printf("Enter a number of quantity for distribution: ");
          scanf("%f", &distQty);
        }

        if (distQty < temp->qty) {
          temp_d = addItemToDistList(temp, temp_d, distQty);
          temp->qty = (temp->qty) - distQty;
        }
        else {
          printf("ERROR: INVALID QUANTITY NUMBER.\n");
        }
      }

      if (temp->next != NULL) {
        temp = temp->next;
      }
      else {
        break;
      }
    }

    modifyDonationFile(head);

    printf("Modified data successfully :)\n");
    
    return temp_d;
  }
  else {
    printf("ERROR: EMPTY LIST.\n");
  }
}

// Add distributed item to DistN Linked List
DistN* addItemToDistList(DonationN* ptr, DistN* ptr_d, float distQty) {

  char destination[20];
  int id_d;
  
  printf("\nEnter the destination name: ");
  scanf("%s", &destination);

  DistN* newDistNode = malloc(sizeof(DistN));

  newDistNode->id_d = rand();
  strcpy(newDistNode->code_d, ptr->code);
  strcpy(newDistNode->dest_d, destination);
  newDistNode->qty_d = distQty;

  printf("\n--- ADDED ---\n");
  printf("ID:%d Supply Code : %s Destination : %s Quantity Distributed : %.2f\n\n",
    newDistNode->id_d, newDistNode->code_d, newDistNode->dest_d, newDistNode->qty_d);
  
  newDistNode->next = NULL;
  newDistNode->prev = NULL;

  return newDistNode;
}



// Modify donation.txt  ----------------------------------
void modifyDonationFile(DonationN* head) {
  FILE* fp;
  fp = fopen("donation.txt", "w");

  if (fp != NULL) {
    DonationN* curr = head;
    DonationN* pnext = NULL;
    DonationN* pprev = NULL;

    while (curr != NULL) {
      pnext = curr->next;
      pprev = curr->prev;

      curr->next = NULL;
      curr->prev = NULL;
      
      fprintf(fp, "\n");
      fprintf(fp, "%d ", curr->id);
      fprintf(fp, "%s ", curr->supply);
      fprintf(fp, "%s ", curr->code);
      fprintf(fp, "%s ", curr->donator);
      fprintf(fp, "%d ", curr->nShip);
      fprintf(fp, "%.2f", curr->qty);

      printf("\nWriting [%d] %s to donation.txt file.......\n", curr->id, curr->supply);

      curr->next = pnext;
      curr->prev = pprev;

      pnext = NULL;
      pprev = NULL;

      curr = curr->next;
    }

    fclose(fp);
    fp = NULL;
  }
  else {
    printf("ERROR: FILE CAN'T OPEN FOR MODIFYING.\n");
  }
}


// Desc Bubble sort for Donation.txt ----------------------------------
void orderDonationList(DonationN* head) {
  
  DonationN* temp = head;
  int tempId, tempShip, flag = 0;
  char tempSupply[30], tempCode[5], tempDonator[20];
  float tempQty;
  
  if (temp != NULL) {
    
    while (flag == 0) {
      flag = 1;
      temp = head;

      while (temp->next != NULL) {
        
        if (temp->qty < temp->next->qty) {
          tempId = temp->next->id;
          temp->next->id = temp->id;
          temp->id = tempId;
          
          tempQty = temp->next->qty;
          temp->next->qty = temp->qty;
          temp->qty = tempQty;
          
          strcpy(tempSupply, temp->next->supply);
          strcpy(temp->next->supply, temp->supply);
          strcpy(temp->supply, tempSupply);

          strcpy(tempCode, temp->next->code);
          strcpy(temp->next->code, temp->code);
          strcpy(temp->code, tempCode);
          
          strcpy(tempDonator, temp->next->donator);
          strcpy(temp->next->donator, temp->donator);
          strcpy(temp->donator, tempDonator);

          tempShip = temp->next->nShip;
          temp->next->nShip = temp->nShip;
          temp->nShip = tempShip;
          
          flag = 0;
        }
        
        temp = temp->next;
      }
    }

  }
  else {
    printf("ERROR: Linked List is empty.\n");
  }
}


// Desc Bubble sort for Dist.txt ===============
DistN* orderDistList(DistN* head_d) {
  DistN* temp = head_d;
  int tempId, flag = 0;
  char tempCode[5], tempDest[20];
  float tempQty;
  
  if (temp != NULL) {
    
    while (flag == 0) {
      flag = 1;
      temp = head_d;

      while (temp->next != NULL) {
        if (temp->qty_d < temp->next->qty_d) {
          tempId = temp->next->id_d;
          temp->next->id_d = temp->id_d;
          temp->id_d = tempId;
          
          tempQty = temp->next->qty_d;
          temp->next->qty_d = temp->qty_d;
          temp->qty_d = tempQty;

          strcpy(tempCode, temp->next->code_d);
          strcpy(temp->next->code_d, temp->code_d);
          strcpy(temp->code_d, tempCode);
          
          strcpy(tempDest, temp->next->dest_d);
          strcpy(temp->next->dest_d, temp->dest_d);
          strcpy(temp->dest_d, tempDest);
          
          flag = 0;
        }
        temp = temp->next;
      }
    }

  }
  else {
    printf("ERROR: Linked List is empty.\n");
  }
}


// Search ----------------------------------
void searchDonation(DonationN* head) {

  int targetId;
  int flag;
  DonationN* temp = head;

  if (temp != NULL) {
    printf("\nEnter ID of the donation supply: ");
    scanf("%d", &targetId);

    while (temp != NULL) {
      if (temp->id == targetId) {
        printf("ID: %d Name of Supply: %s Supply Code: %s Donator: %s Number of Shipment: %d Quantity Received: %.2f\n",
          temp->id, temp->supply, temp->code, temp->donator, temp->nShip, temp->qty);

        flag = 0;
        break;
      }
      else {
        flag = 1;
      }

      temp = temp->next;
    }

    if (flag == 1) {
      printf("ERROR: ITEM NOT FOUND.\n");
    }
    
  }
  else {
    printf("ERROR: EMPTY LIST.\n");
  }
}


// Search dist.txt ===============
void searchDist(DistN* head_d) {
  int targetId;
  int flag;
  DistN* temp = head_d;

  if (temp != NULL) {
    printf("\nEnter ID of the donation supply: ");
    scanf("%d", &targetId);

    while (temp != NULL) {
      if (temp->id_d == targetId) {
        printf("ID: %d Supply Code: %s Donator: %s Quantity Received: %.2f\n",
          temp->id_d, temp->code_d, temp->dest_d, temp->qty_d);

        flag = 0;
        break;
      }
      else {
        flag = 1;
      }
      
      temp = temp->next;
    }

    if (flag == 1) {
      printf("ERROR: ITEM NOT FOUND.\n");
    }
    
  }
  else {
    printf("ERROR: EMPTY LIST.\n");
  }

}


// Search total quantity ----------------------------------
void searchDonationTotal(DonationN* ptr) {

  char targetCode[10];
  float totalQty = 0;
  int i = 0;
  DonationN* temp = ptr;

  if (temp != NULL) {
    printf("\n## SUPPLY CODE ##\n");
    printf("CT - Contactless Thermometer\n");
    printf("HS - Hand Sanitizer\n");
    printf("FM - Face Mask\n");
    printf("SM - Surgical Mask\n");
    printf("OM - Oxygen Mask\n");
    
    printf("\nEnter code of the donation supply: ");
    scanf("%s", &targetCode);
    printf("\n");

    makeUpper(targetCode);

    while (temp != NULL) {
      if (strcmp(temp->code, targetCode) == 0) {
        printf("ID: %d Name of Supply: %s Supply Code: %s Donator: %s Number of Shipment: %d Quantity Received: %.2f\n",
          temp->id, temp->supply, temp->code, temp->donator, temp->nShip, temp->qty);

        totalQty += temp->qty;
      }

      temp = temp->next;
    }

    printf("\nTOTAL QUANTITY: %.2f\n\n", totalQty);
  }
  else {
    printf("ERROR: Cannot find the supply.\n");
  }
}



void searchDistTotal(DistN* ptr_d) {
  
  char targetCode[10];
  float totalQty = 0;
  int i = 0;
  DistN* temp = ptr_d;

  if (temp != NULL) {
    printSupplyCode();

    printf("Enter code of the donation supply: ");
    scanf("%s", &targetCode);
    printf("\n");

    makeUpper(targetCode);

    while (temp != NULL) {
      if (strcmp(temp->code_d, targetCode) == 0) {
        printf("ID: %d Supply Code: %s Destination: %s Quantity Received: %.2f\n",
          temp->id_d, temp->code_d, temp->dest_d, temp->qty_d);

        totalQty += temp->qty_d;
      }

      temp = temp->next;
    }

    printf("\nTOTAL QUANTITY: %.2f\n\n", totalQty);
  }
  else {
    printf("ERROR: Cannot find the supply.\n");
  }
}


// Clean up the memory ----------------------------------
void cleanUpMemory(DonationN* ptr) {
  DonationN*  freeThis = ptr;
  DonationN* hold = freeThis;//both hold and toBeFree pinters have ptr

  //only when toBeFree is not NULL to avoid an error
  while (freeThis != NULL) {
    hold = freeThis->next;
    free(freeThis);//emptying the memory toBeFree is pointing
    freeThis = hold;//hold pointergives ptr to the memory toBeFree is pointing
  }
}

// Clean up the memory of DistN ===============
void cleanUpDistMemory(DistN* ptr) {
  DistN*  freeThis = ptr;
  DistN* hold = freeThis;

  while (freeThis != NULL) {
    hold = freeThis->next;
    free(freeThis);
    freeThis = hold;
  }
}



//Main function --------------------------------------------
int main() {
  int mainMenuInp, searchChoice, flag = 0;
  srand(time(NULL));
  
  DonationN* head = NULL;
  DistN* head_d = NULL;

  while (flag == 0) {
    mainMenu();
    scanf("%d", &mainMenuInp);

      if (mainMenuInp == 1) {
        printf("----- Create Donation Details -----\n");
        writeListToDonationFile(createNode(head));
      }
      else if (mainMenuInp == 2) {
        printf("----- Distribute Supply -----\n");
        head = readLinkedList(head);
        head_d = readDistLinkedList(head_d);
        orderDonationList(head);
        printList(head);
        head_d = subtractFromDist(head, head_d);
        writeListToDistFile(head_d);
      }
      else if (mainMenuInp == 3) {
        printf("----- Search Details -----\n");
        searchChoice = searchMenu();

        if (searchChoice == 1) {
          printf("----- Search Donation Details -----\n");
          head = readLinkedList(head);
          printList(head);
          searchDonation(head);
        }
        else if (searchChoice == 2) {
          printf("----- Search Distribution Details -----\n");
          head_d = readDistLinkedList(head_d);
          printDistList(head_d);
          searchDist(head_d);
        }
        else if (searchChoice == 3) {
          printf("----- Search Total Quantity of Specific Donation Supply -----\n");
          head = readLinkedList(head);
          searchDonationTotal(head);
        }
        else if (searchChoice == 4) {
          printf("----- Search Total Quantity of Specific Distributed Supply -----\n");
          head_d = readDistLinkedList(head_d);
          searchDistTotal(head_d);
          continue;
        }
        else if (searchChoice == 5) {
          printf("----- Back to Main Menu -----\n");
        }
        else {
          printf("\n!! Please choose the number from the menu.\n");
        }
      }
      else if (mainMenuInp == 4) {
        printf("----- List All Distributed Donations & Accumulated Quantities -----\n");
        head = readLinkedList(head);
        head_d = readDistLinkedList(head_d);
        orderDonationList(head);
        printf("\n----- Donation Supply ----- \n");
        printList(head);
        orderDistList(head_d);
        printf("\n----- Distributed Supply ----- \n");
        printDistList(head_d);
      }
      else if (mainMenuInp == 5) {
        printf("----- Quit -----\n\n");
        flag = 1;
        break;
      }
      else {
        printf("\n!! Please choose the valid number from the menu.\n");
      }

  }

  cleanUpMemory(head);
  cleanUpDistMemory(head_d);

  return 0;
}
