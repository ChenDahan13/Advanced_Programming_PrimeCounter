#include "myprimeCounter.h"

int total_prime_numbers = 0;

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

PNode newNode(int* numbers, int size) {
    PNode node = (PNode)malloc(sizeof(Node));
    node->numbers = numbers;
    node->size = size;
    node->next = NULL;
    return node;
}

// Iinitialize a new queue
PQueue newQueue() {
    PQueue queue = (PQueue)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    pthread_mutex_init(&queue->lock, NULL);
    return queue;
}

// Add sequence of numbers to the queue
void enqueue(PQueue queue, int* numbers, int size) {
    if (queue == NULL || numbers == NULL || size == 0) return; // Invalid input
    
    pthread_mutex_lock(&queue->lock); // Lock the queue
    
    int* numbersCopy = (int*)malloc(size * sizeof(int)); // Copy the numbers to new array
    for (int i = 0; i < size; i++) {
        numbersCopy[i] = numbers[i];
    }

    PNode node = newNode(numbersCopy, size); // Create a new node
    
    if (queue->head == NULL) { // Adds first node
        queue->head = node;
        queue->tail = node;
    } else { // Adds to the end of the queue
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
    pthread_mutex_unlock(&queue->lock);
}

// Return sequence of numbers from the queue
PNode dequeue(PQueue queue) {
    if (queue == NULL) return NULL; // Invalid input
    
    pthread_mutex_lock(&queue->lock); // Lock the queue

    if (queue->head == NULL) { // Empty queue
        pthread_mutex_unlock(&queue->lock);
        return NULL;
    }
    
    PNode node = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    if (queue->head == NULL) { // Last node
        queue->tail = NULL;
    }
    
    pthread_mutex_unlock(&queue->lock);
    
    return node;
}

// Free the queue
void freeQueue(PQueue queue) {
    if (queue == NULL) return; // Invalid input

    while (queue->head != NULL) {
        PNode node = queue->head;
        queue->head = queue->head->next;
        free(node->numbers);
        free(node);
    }
    pthread_mutex_destroy(&queue->lock);
    free(queue);
}

// Thread function to check if numbers are prime
void *thread_func(PQueue queue) {
    PNode node;
    int counter = 0;
    
    while ((node = dequeue(queue)) != NULL) { 
        for (int i = 0; i < node->size; i++) {
            if (isPrime(node->numbers[i])) {
                counter++;
            }
        }
        free(node->numbers);
        free(node);

        pthread_mutex_lock(&queue->lock);
        total_prime_numbers += counter; // Update the total prime numbers
        pthread_mutex_unlock(&queue->lock);
        counter = 0;    
    }
    
    
}

int main() {
    PQueue queue = newQueue();
    int num;
    int* numbers = (int*)malloc(MAX_NUMBERS * sizeof(int)); // Array to store numbers to check
    int size = 0;
    pthread_t threads[MAX_THREADS];
    
    
    // Read numbers from stdin until end of file, spilt the numbers into chunks
    while (scanf("%d", &num) != EOF) {
        numbers[size++] = num;
        if (size == MAX_NUMBERS) {
            enqueue(queue, numbers, size);
            size = 0;
        }
    }
    
    // Add the remaining numbers
    if (size > 0) {
        enqueue(queue, numbers, size);
    }

    free(numbers);

    // Create threads
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, (void*) thread_func, queue);
    }

    // Wait for threads to finish
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_prime_numbers);

    freeQueue(queue);
    return 0;
}
