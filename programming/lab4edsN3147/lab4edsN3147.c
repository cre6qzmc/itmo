#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_LINE_LENGTH 1024

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t size;
} List;

void print_usage() {
    fprintf(stderr, "Usage: ./lab4abcNXXXXX [-v] filename\n");
}

void print_version() {
    printf("Щенникова Елена Дмитриевна, гр. N3147\nВариант: 1-10-19-4\n");
}

void init_list(List *list) {
    list->head = NULL;
    list->size = 0;
}

void free_list(List *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->size = 0;
}

bool is_valid_license_plate(char *str) {
    size_t len = strlen(str);
    if (len < 8 || len > 9) {
        return false;
    }

    char* valid_letters = "ABEKMHOPCTYX";
    if (strchr(valid_letters, str[0]) == NULL ||
        strchr(valid_letters, str[4]) == NULL ||
        strchr(valid_letters, str[5]) == NULL) {
        return false;
    }

    for (int i = 1; i <= 3; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }

    for (int i = 6; i <= 7; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }

    if (len == 9 && (str[8] < '0' || str[8] > '9')) {
        return false;
    }

    return true;
}

void push_front(List *list, const char *data) {
    if (!is_valid_license_plate((char *)data)) {
        fprintf(stderr, "Неправильный формат строки: %s\n", data);
        return;
    }
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = strdup(data);
    node->next = list->head;
    list->head = node;
    list->size++;
}

void push_back(List *list, const char *data) {
    if (!is_valid_license_plate((char *)data)) {
        fprintf(stderr, "Неправильный формат строки: %s\n", data);
        return;
    }
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = strdup(data);
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
    list->size++;
}

void pop_front(List *list) {
    if (list->head == NULL) return;
    Node *temp = list->head;
    list->head = list->head->next;
    free(temp->data);
    free(temp);
    list->size--;
}

void pop_back(List *list) {
    if (list->head == NULL) return;
    if (list->head->next == NULL) {
        free(list->head->data);
        free(list->head);
        list->head = NULL;
    } else {
        Node *current = list->head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next->data);
        free(current->next);
        current->next = NULL;
    }
    list->size--;
}

void dump(List *list, const char *filename) {
    FILE *fp = filename ? fopen(filename, "w") : stdout;
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        fprintf(fp, "%p %p %s\n", (void *)current, (void *)current->next, current->data);
        current = current->next;
    }
    if (filename) fclose(fp);
}

void insert(List *list, size_t pos, const char *data) {
    if (!is_valid_license_plate((char *)data)) {
        fprintf(stderr, "Неправильный формат строки: %s\n", data);
        return;
    }
    if (pos > list->size) return;
    if (pos == 0) {
        push_front(list, data);
        return;
    }
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = strdup(data);
    Node *current = list->head;
    for (size_t i = 0; i < pos - 1; ++i) {
        current = current->next;
    }
    node->next = current->next;
    current->next = node;
    list->size++;
}

void handle_command(List *list, char *command) {
    if (strncmp(command, "push_front", 10) == 0) {
        char *args = command + 11; 
        char *token = strtok(args, ", ");
        char **tmptok = (char **)malloc((100) * sizeof(char*));
        int cnttmp = 0;
    	while (token) {
        	tmptok[cnttmp] = strdup(token);
        	cnttmp++;
			token = strtok(NULL, ", ");
    	}
    	cnttmp = cnttmp - 1;
        while (cnttmp >=0) {
            push_front(list, tmptok[cnttmp]); // Добавляем строку в начало списка
            cnttmp = cnttmp - 1;
        }
        free(tmptok);
    } else if (strncmp(command, "push_back", 9) == 0) {
            // Обработка команды push_back
            
            char *args = command + 10;
            // Пропуск пробелов и кавычек
            char *token = strtok(args, ", ");
            while (token) {
                push_back(list, token); // Добавляем строку в конец списка
                token = strtok(NULL, ", ");
            }
        } else if (strcmp(command, "pop_front") == 0) {
            pop_front(list); // Удаляем первый элемент списка
        } else if (strcmp(command, "pop_back") == 0) {
            pop_back(list); // Удаляем последний элемент списка
        } else if (strncmp(command, "dump", 4) == 0) {
            char *args = command + 5;
            char *output_file = strtok(args, " ");
            if (output_file) {
                dump(list, output_file); // Выводим список в указанный файл
            } else {
                dump(list, NULL); // Выводим список на экран
            }
        } else if (strncmp(command, "insert", 6) == 0) {
            char *args = command + 7;
            size_t pos;
			char data[MAX_LINE_LENGTH];
			if (sscanf(args, "%zu %[^\n]", &pos, data) == 2) {
				insert(list, pos, data);
			} else {
				fprintf(stderr, "Invalid insert command format\n");
			}
        } else {
            fprintf(stderr, "Ошибка: неподдерживаемая команда: '%s'\n", command); // Выводим сообщение об ошибке для неподдерживаемых команд
        }
    }

void load_list_from_file(List *list, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) return;
    unsigned int cnt;
    fread(&cnt, sizeof(cnt), 1, fp);
    unsigned short *indices = malloc(sizeof(unsigned short) * cnt);
    unsigned int *offsets = malloc(sizeof(unsigned int) * cnt);
    if (!indices || !offsets) {
        perror("Failed to allocate memory for indices or offsets");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < cnt; ++i) {
        fread(&indices[i], sizeof(unsigned short), 1, fp);
        fread(&offsets[i], sizeof(unsigned int), 1, fp);
    }
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    char *buffer = malloc(file_size);
    if (!buffer) {
        perror("Failed to allocate memory for buffer");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_SET);
    fread(buffer, 1, file_size, fp);
    for (unsigned int i = 0; i < cnt; ++i) {
        if (is_valid_license_plate(buffer + offsets[i])) {
            push_back(list, buffer + offsets[i]);
        }
    }
    free(buffer);
    free(indices);
    free(offsets);
    fclose(fp);
}

void save_list_to_file(List *list, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    unsigned int cnt = list->size;
    fwrite(&cnt, sizeof(cnt), 1, fp);
    unsigned short idx = 0;
    unsigned int offset = sizeof(cnt) + cnt * (sizeof(unsigned short) + sizeof(unsigned int));
    Node *current = list->head;
    while (current != NULL) {
        fwrite(&idx, sizeof(idx), 1, fp);
        fwrite(&offset, sizeof(offset), 1, fp);
        offset += strlen(current->data) + 1;  // +1 for null terminator
        idx++;
        current = current->next;
    }
    current = list->head;
    while (current != NULL) {
        fwrite(current->data, strlen(current->data) + 1, 1, fp);  // +1 for null terminator
        current = current->next;
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
	
	char *DEBUG = getenv("LAB4DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }
	
    if (argc < 2) {
        print_usage();
        return 1;
    }
    else {
		if (strcmp(argv[1], "-v") == 0) { 
			print_version();
			return 0;
		}
		else if (argc == 2 && argv[1][0] == '-' && argv[1][1] != 'v') {
			fprintf(stderr, "Ошибка: неподдерживаемая опция %s.\n", argv[1]);
			return 1;
		}
	}
	
    const char *filename = argv[1];
    List list;
    init_list(&list);
    load_list_from_file(&list, filename);

    char command[MAX_LINE_LENGTH];
    while (fgets(command, sizeof(command), stdin)) {
		command[strcspn(command, "\n")] = '\0';
        handle_command(&list, command);
    }
    
    save_list_to_file(&list, "result.bin");
    free_list(&list);
    return 0;
}