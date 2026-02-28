/* 
Một cầu chỉ chịu được tải trọng tối đa là K (số người). 
Một nhóm N người cần đi qua cầu trong thời gian tối thiểu. 
Mỗi người trong nhóm có thời gian đi qua cầu khác nhau được cung cấp trong một mảng số nguyên có độ dài là N. 
Viết chương trình C lên kế hoạch cho N người qua cầu theo thứ tự sao cho tổng thời gian qua cầu là nhỏ nhất.
INPUT: Hàng thứ nhất là trọng tải cầu K, hàng thứ hai là số người N, hàng thứ ba là mảng số nguyên lưu trữ thời gian mỗi người qua cầu tương ứng.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int head, tail;
    int data[MAX];
} Queue;

void init(Queue* q) {
    q->head = 0;
    q->tail = -1;
}

int isFull(Queue *q) {
    return (q->head == 0 && q->tail == MAX - 1);
}

int isEmpty(Queue *q) {
    return (q->head > q->tail);
}

void put(Queue* q, int value) {
    if (isFull(q)) {
        return;
    }

    q->tail++;
    q->data[q->tail] = value;
}

int get(Queue* q) {
    if (isEmpty(q)) {
        return 0;
    }

    int value = q->data[q->head];
    q->head++;

    return value;
}

// Sắp xếp mảng giảm dần (Insertion Sort)
void sortDescending(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;
    }
}

int crossingBridge(int times[], int n, int k) {
    // Sắp xếp giảm dần để người chậm đi trước
    sortDescending(times, n);
    
    // Queue người chờ và người đang qua cầu
    Queue waiting, crossing;
    init(&waiting);
    init(&crossing);

    // Thêm người vào hàng chờ
    for (int i = 0; i < n; i++) {
        put(&waiting, times[i]);
    }
    
    // Trường hợp đặc biệt: n <= k
    if (n <= k) {
        printf("\n=== CROSSING PROCESS ===\n");
        int t = 0;
        while (!isEmpty(&waiting)) {
            t++;
            printf("t = %d: ", t);
            
            for (int i = 0; i < n; i++) {
                printf("%d ", times[i]);
            }
            printf("\n");

            if (t == times[0]) break;
        }   
        
        printf("DONE!\n"); 
        return times[0];
    }

    printf("\n=== CROSSING PROCESS ===\n");
    int remainingTime[MAX], time = 0;
        
    // Thêm k người đầu tiên lên cầu
    for (int i = 0; i < k && !isEmpty(&waiting); i++) {
        int person = get(&waiting);
        put(&crossing, person);
        remainingTime[i] = person;
    }
    
    while (!isEmpty(&crossing)) {
        time++;
        printf("time = %d: ", time);

        // In người đang trên cầu
        int count = 0;
        for (int i = crossing.head; i <= crossing.tail; i++) {
            printf("%d ", crossing.data[i]);
            count++;
        }
        printf("\n");

        // Giảm thời gian và xử lý
        int newRemaining[MAX], newCount = 0;
        Queue newCrossing;
        init(&newCrossing);

        for (int i = 0; i < count; i++) {
            remainingTime[i]--;
            if (remainingTime[i] > 0) {
                put(&newCrossing, crossing.data[crossing.head + i]);
                newRemaining[newCount++] = remainingTime[i];
            } else if (!isEmpty(&waiting)) {
                int person = get(&waiting);
                put(&newCrossing, person);
                newRemaining[newCount++] = person;
            }
        }

        // Thêm người mới vào chỗ trống
        while (newCount < k && !isEmpty(&waiting)) {
            int person = get(&waiting);
            put(&newCrossing, person);
            newRemaining[newCount++] = person;
        }

        // Cập nhật
        crossing = newCrossing;
        for (int i = 0; i < newCount; i++) {
            remainingTime[i] = newRemaining[i];
        }
    }

    printf("DONE!\n");
    return time;
}


int main() {
    int k, n, times[MAX];

    printf("Enter the load capacity of the bridge: k = ");
    scanf("%d", &k);

    printf("Enter the number of people: n = ");
    scanf("%d", &n);

    printf("Time per person to cross the bridge: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &times[i]);
    }

    int minTime = crossingBridge(times, n, k);

    printf("\n>> THE MINIMUM TIME: %d", minTime);

    return 0;
}

