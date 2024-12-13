#include <stdio.h>
#include <string.h>
#define n 100 

void PrintMenu();
void Menu();
void AddSubscriber(struct abonent *arr, int size);
void DeleteSubscriber(struct abonent *arr, int size);
void DisplaySubscribers(struct abonent *arr, int size);
void SearchByName(struct abonent *arr, int size);

typedef struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
} abonent;

abonent abonentArr[n]; 

int main() {
    Menu();
    return 0;
}

void PrintMenu() {
    printf("   ===Menu=== \n"
           "1) Add a subscriber\n"
           "2) Delete a subscriber\n"
           "3) Search subscribers by name\n"
           "4) Display all records\n"
           "5) Exit\n");
}

void Menu() {
    while (1) {
        int select = 0;

        PrintMenu();

        printf("Select section: ");
        scanf("%d", &select);

        switch (select) {
            case 1:
                AddSubscriber(abonentArr, n);
                break;
            case 2:
                DeleteSubscriber(abonentArr, n);
                break;
            case 3:
                SearchByName(abonentArr, n); 
                break;
            case 4:
                DisplaySubscribers(abonentArr, n);
                break;
            case 5:
                printf("Exiting...\n");
                return; 
            default:
                printf("Invalid selection, please try again.\n");
                break;
        }
    }
}

void AddSubscriber(struct abonent *arr, int size) {
    // Находим первый незаполненный элемент
    for (int i = 0; i < size; i++) {
        if (strlen(arr[i].name) == 0 && strlen(arr[i].second_name) == 0 && strlen(arr[i].tel) == 0) {
            // Заполняем только первый найденный пустой элемент
            printf("Enter name for subscriber: ");
            scanf("%9s", arr[i].name);

            printf("Enter second name for subscriber: ");
            scanf("%9s", arr[i].second_name); 

            printf("Enter telephone number for subscriber: ");
            scanf("%9s", arr[i].tel); 

            printf("Subscriber added successfully at index %d.\n", i);
            return; 
        }
    }

    // Если свободных слотов нет
    printf("No empty subscriber slots available.\n");
}
void DeleteSubscriber(struct abonent *arr, int size) {
    int index;
    printf("Enter index of subscriber to delete (0 to %d): ", n-1);
    scanf("%d", &index);

    if (index >= 0 && index < size) {

        abonentArr[index].name[0] = '\0';
        abonentArr[index].second_name[0] = '\0';
        abonentArr[index].tel[0] = '\0';

        printf("Subscriber at index %d deleted successfully.\n", index);
    } else {
        printf("Invalid index. Please enter a value between 0 and %d.\n", n-1);
	}
}
void SearchByName(struct abonent *arr, int size) {
    char name[10];  
    printf("Enter name: ");
    scanf("%9s", name); 

    for (int i = 0; i < size; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            printf("Subscriber found:\n");
            printf("  Name: %s\n", arr[i].name);
            printf("  Second Name: %s\n", arr[i].second_name);
            printf("  Telephone: %s\n", arr[i].tel);
            return;
        }
    }
    printf("Subscriber with name \"%s\" not found.\n", name);
}  
void DisplaySubscribers(struct abonent *arr, int size) {
    printf("=== Subscribers List ===\n");
    for (int i = 0; i < size; i++) {
        printf("Subscriber %d:\n", i + 1);
        printf("  Name: %s\n", abonentArr[i].name);
        printf("  Second Name: %s\n", abonentArr[i].second_name);
        printf("  Telephone: %s\n", abonentArr[i].tel);
    }
}