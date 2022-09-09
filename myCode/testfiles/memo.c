#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct donation {
  int id;
  char supply[30];
  char code[5];
  char donator[10];
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

  struct distribution* next;//point to the next node
  struct distribution *prev;//point to the previous node
} DistN;


// Function Prototype ----------------------------------
void mainMenu();
int searchMenu();
int deleteMenu();

DonationN* createNode();
DonationN* addItemToList();
void printList();
DonationN *ReadLinkedList();
void writeListToDonationFile();
void addQuantity();
DistN* subtractFromDist();
DistN* addItemToDistList();
void modifyDonationFile();
void searchDonation();
DonationN* deleteList();
void cleanUpMemory();

DistN* createDistNode();
void printDistList();
DistN* readDistLinkedList();
void writeListToDistFile();
void searchDist();
void cleanUpDistMemory();
DistN *modifyDistList();

// Display menu ----------------------------------
void mainMenu() {
  printf("\n====================================================\n");
  printf("COVID-19 DONATION MANAGEMENT SYSTEM\n");
  printf("====================================================\n");
  printf("\n1 - Create Donation Details\n");
  printf("2 - Update Donation quantities\n");
  printf("3 - Search Details\n");
  printf("4 - Delete Details\n");
  printf("5 - List all Distributed Donation & Accumulated Quantities\n");
  printf("6 - Quit\n");
  printf("\n----------------------------------------------------\n\n");
}

int updateDonationMenu() {
  int choice;
  printf("\n1 - Add Quantity To Existing Record\n");
  printf("2 - Distribute Supply\n");
  printf("3 - Back To Main Menu\n");
  printf("\n----------------------------------------------------\n\n");
  scanf("%d", &choice);

  return choice;
}

int searchMenu() {
  int choice;
  printf("\n1 - Search Donation Details\n");
  printf("2 - Search Distribution Details\n");
  printf("3 - Back To Main Menu\n");
  printf("\n----------------------------------------------------\n\n");
  scanf("%d", &choice);

  return choice;
}


int deleteMenu() {
  int choice;
  printf("\n1 - Delete Donation Details\n");
  printf("2 - Delete Distribution Details\n");
  printf("3 - Back To Main Menu\n");
  printf("\n----------------------------------------------------\n\n");
  scanf("%d", &choice);

  return choice;
}


// Create new DonationN node ----------------------------------
DonationN* createNode() {
  char supply[30], code[5], donator[10];
  int nShip;
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

  printf("\nTotal items in the Donation linked list: %d\n\n", count);
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

  printf("\nTotal items in the Distribution linked list: %d\n\n", count);
}

//Read all items on donation.txt to Linked List ----------------------------------
DonationN* readLinkedList(DonationN* head) {
  
  FILE* fp;
  fp = fopen("donation.txt", "r");
  int len, i, fileEmp = 0;

  DonationN* ptr = head;
  DonationN* plast;

  fseek(fp, 0, SEEK_END);
  
  if (ptr == NULL) {
    ptr = malloc(sizeof(DonationN));
    head = ptr;
  }

  len = (int)ftell(fp);
  
  if (len <= 0) {
    fileEmp = 1;
    printf("File is empty.\n");
    ptr = NULL;
    head = ptr;
  }

  if (fileEmp == 0) {
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
  int len, i, fileEmp = 0;

  DistN* ptr = head_d;
  DistN* plast;

  fseek(fp, 0, SEEK_END);
  
  if (ptr == NULL) {
    ptr = malloc(sizeof(DistN));
    head_d = ptr;
  }

  len = (int)ftell(fp);
  
  if (len <= 0) {
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
      fprintf(fp, "%.2f ", curr->qty);
      
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
      fprintf(fp, "%.2f ", curr->qty_d);

      printf("\nWriting [%d] %s to dist.txt file.......\n", curr->id_d, curr->dest_d);

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
    printf("ERROR: FILE CAN'T OPEN FOR WRITING.\n");
  }
}



// Add Quantity of Received Item ----------------------------------
void addQuantity(DonationN* head) {

  float addNum;
  int targetId;

  DonationN* temp = head;
  DonationN* pnext;

  if (temp != NULL) {
    printf("\nEnter ID of the donation supply: ");
    scanf("%d", &targetId);

    while (temp != NULL) {
      if (temp->id == targetId) {
        printf("ID: %d Name of Supply: %s Supply Code: %s Donator: %s Number of Shipment: %d Quantity Received: %.2f\n",
          temp->id, temp->supply, temp->code, temp->donator, temp->nShip, temp->qty);

        printf("\nEnter a number of quantity received: ");
        scanf("%f", &addNum);
        
        if (0 < addNum) {
          temp->qty = (temp->qty) + addNum;
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
    
    modifyDonationFile(temp);
    printf("Modified data successfully :)\n");
    return;
  }
  else {
    printf("ERROR: EMPTY LIST.\n");
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
          head_d = addItemToDistList(temp, temp_d, distQty);
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

    modifyDonationFile(temp);

    printf("Modified data successfully :)\n");
    return head_d;
  }
  else {
    printf("ERROR: EMPTY LIST.\n");
  }
}

// Add distributed item to DistN Linked List
DistN* addItemToDistList(DonationN *ptr, DistN *ptr_d, float distQty) {

  char destination[10];
  int id_d;
  
  printf("\nEnter the destination name: ");
  scanf("%s", &destination);

  DistN* newDistNode = malloc(sizeof(DistN));

  newDistNode->id_d = rand();
  strcpy(newDistNode->code_d, ptr->code);
  strcpy(newDistNode->dest_d, destination);
  printf("Test1.5 in addItemToDistList()...\n");
  printf("Test: %f\n", distQty);
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

      fprintf(fp, "%d ", curr->id);
      fprintf(fp, "%s ", curr->supply);
      fprintf(fp, "%s ", curr->code);
      fprintf(fp, "%s ", curr->donator);
      fprintf(fp, "%d ", curr->nShip);
      fprintf(fp, "%.2f", curr->qty);
      fprintf(fp, "\n");

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


// Modify dist.txt  ----------------------------------
DistN *modifyDistList(DistN *head_d) {
  FILE* fp;
  fp = fopen("dist.txt", "w");

  if (fp != NULL) {
    DistN* curr = head_d;
    DistN* pnext = NULL;
    DistN* pprev = NULL;

    while (curr != NULL) {
      pnext = curr->next;
      pprev = curr->prev;

      curr->next = NULL;
      curr->prev = NULL;

      fprintf(fp, "%d ", curr->id_d);
      fprintf(fp, "%s ", curr->code_d);
      fprintf(fp, "%d ", curr->dest_d);
      fprintf(fp, "%.2f", curr->qty_d);
      fprintf(fp, "\n");

      printf("\nWriting [%d] %s %d to dist.txt file.......\n", curr->id_d, curr->code_d,curr->dest_d);

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

  return head_d;
}

// Desc Bubble sort for Donation.txt ----------------------------------
void orderDonationList(DonationN* head) {
  
  DonationN* temp = head;
  int tempQty;
  int flag = 0;
  
  if (temp != NULL) {
    
    while (flag == 0) {
      flag = 1;
      temp = head;

      while (temp->next != NULL) {
        if (temp->qty > temp->next->qty) {
          tempQty = temp->next->qty;
          temp->next->qty = temp->qty;
          temp->qty = tempQty;
          flag = 0;
        }
        temp = temp->next;
      }
    }
    
    for (int i = 0; temp != NULL; i++) {
      printList(temp);
      temp = temp->next;
    }

  }
  else {
    printf("ERROR: Linked List is empty.\n");
  }
}


// Desc Bubble sort for Dist.txt ===============
DistN* orderDistList(DistN* head_d) {
  DistN* temp = head_d;
  int tempQty;
  int flag = 0;
  
  if (temp != NULL) {
    
    while (flag == 0) {
      flag = 1;
      temp = head_d;

      while (temp->next != NULL) {
        if (temp->qty_d > temp->next->qty_d) {
          tempQty = temp->next->qty_d;
          temp->next->qty_d = temp->qty_d;
          temp->qty_d = tempQty;
          flag = 0;
        }
        temp = temp->next;
      }
    }
    
    for (int i = 0; temp != NULL; i++) {
      printList(temp);
      temp = temp->next;
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


// Delete item on Linked List ----------------------------------
DonationN* deleteList(DonationN* phead) {
  int inputId;
  
  printf("Enter ID to delete: ");
  scanf("%d", &inputId);

  DonationN* itemRef = phead;
  DonationN* deleteThis = NULL;

  while (itemRef != NULL) {
    if (inputId == itemRef->id) {
      deleteThis = itemRef;
      break;
    }
    itemRef = itemRef->next;
  }

  if (phead != NULL && deleteThis == phead) {
    
    if (deleteThis->next != NULL) {
      deleteThis->next->prev = NULL;
      phead = deleteThis->next;
    }
    else {
      phead = NULL;
    }
    
  }
  else {
    
    if (deleteThis != NULL) {
      if (deleteThis->prev != NULL) {
        deleteThis->prev->next = deleteThis->next;
      }

      if (deleteThis->next != NULL) {
        deleteThis->next->prev = deleteThis->prev;
      }
      
    }
    
  }

  if (deleteThis != NULL) {
    deleteThis = NULL;
    free(deleteThis);
  }

  return phead;
}


// Clean up the memory ----------------------------------
void cleanUpMemory(DonationN* ptr) {
  DonationN*  freeThis = ptr;
  DonationN* hold = freeThis;//both hold and toBeFree pinters have ptr

  //only when toBeFree is not NULL to avoid an error
  while (freeThis != NULL) {
    hold = freeThis->next;
    printf("Freeing - ID: %d Name of Supply: %s Supply Code: %s Donator: %s Number of Shipment: %d Quantity Received: %.2f\n\n",
      freeThis->id, freeThis->supply, freeThis->code, freeThis->donator, freeThis->nShip, freeThis->qty);
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
    printf("Freeing - ID: %d Supply Code: %s Destination: %s  Quantity Distributed: %.2f\n\n",
      freeThis->id_d, freeThis->code_d, freeThis->dest_d, freeThis->qty_d);
    free(freeThis);
    freeThis = hold;
  }
}





//Main function --------------------------------------------
int main() {
  int mainMenuInp, flag = 0;
  int updateChoice, searchChoice, deleteChoice;
  DonationN* head = NULL;
  DistN* head_d = NULL;

  while (flag == 0) {
    mainMenu();
    scanf("%d", &mainMenuInp);

      if (mainMenuInp == 1) {
        printf("----- Create Donation Details -----\n");
        head = addItemToList(head);
        printList(head);
        writeListToDonationFile(head);
      }
      else if (mainMenuInp == 2) {
        printf("----- Update Donation Quantities -----\n");
        updateChoice = updateDonationMenu();

        if (updateChoice == 1) {
          head = readLinkedList(head);
          addItemToList(head);
        }
        else if (updateChoice == 2) {
          head = readLinkedList(head);
          head_d = readDistLinkedList(head_d);
          head_d = subtractFromDist(head, head_d);
          writeListToDistFile(head_d);//appending
        }
        else if (updateChoice == 3) {
          printf("----- Back to Main Menu -----\n");
        }
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
          printf("----- Back to Main Menu -----\n");
        }
        else {
          printf("\n!! Please choose the number from the menu.\n");
        }

      }
      else if (mainMenuInp == 4) {
        printf("----- Delete Details -----\n");
        deleteChoice = deleteMenu();

        if (deleteChoice == 1) {
          head = readLinkedList(head);
          head = deleteList(head);
          modifyDonationFile(head);
        }
        else if (deleteChoice == 2) {
          continue;
          //head_d = readDistLinkedList(head_d);
          //head_d = deleteDistList(head_d);
          //modifyDistFile(head_d);
        }
        else if (deleteChoice == 3) {
          printf("----- Back to Main Menu -----\n");
        }
        else {
          printf("\n!! Please choose the number from the menu.\n");
        }

      }
      else if (mainMenuInp == 5) {
        printf("----- List All Distributed Donations & Accumulated Quantities -----\n");
        head = readLinkedList(head);
        head_d = readDistLinkedList(head_d);
        orderDonationList(head);
        printList(head);
        orderDistList(head_d);
        printDistList(head_d);
      }
      else if (mainMenuInp == 6) {
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
