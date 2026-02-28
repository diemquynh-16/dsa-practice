/*
Một khách sạn có 17 tầng, mỗi tầng có 12 phòng được gán mã theo cú pháp: P[số tầng].[số phòng]
Ví dụ: Tầng 3, phòng 3 thì có mã là P3.3.
Mô tả: Có n đoàn khách, mỗi đoàn gồm m người đến check-in nhận phòng. Vì có những khách lẻ đã đặt phòng trước đó nên không thể xếp cho cả 1 đoàn khách ở cùng một tầng, do đó các khách trong đoàn được xếp ngẫu nhiên vào các phòng trống rời rạc sao cho khi truy cập thông tin vào một phòng, ta có thể biết tên (chữ cái đầu) của khách và địa chỉ phòng của người tiếp theo trong đoàn khách. Biết rằng phòng của các trưởng đoàn luôn ở tầng trệt (tầng 0). Giả sử sức chứa của mỗi phòng là 01 người.

- Vấn đề 1: Cho trước trạng thái các phòng hiện tại của khách sạn với ký hiệu “0” tương ứng với “còn trống” và “1” tương ứng với “đã có người”. 
Hãy xếp n đoàn khách vào các phòng sao cho không còn phòng nào “còn trống”.

- Vấn đề 2: Nhân viên tiếp tân muốn xác nhận thông tin của một đoàn ngẫu nhiên thì chỉ cần biết được vị trí phòng của trưởng đoàn. 
Hãy viết một hàm cho phép tiếp tân nhập vào địa chỉ của trưởng đoàn của một đoàn khách bất kỳ, hàm sẽ trả về tất cả thông tin của một đoàn bao gồm tên và vị trí phòng tương ứng của từng khách chung đoàn.

- Vấn đề 3: Biết rằng lúc này có một số khách lẻ check-out nên xuất hiện một số phòng trống. Đột nhiên một số đoàn khách báo cáo với khách sạn về việc bổ sung một số khách mới cần check-in. 
Hãy bổ sung các vị khách này vào các đoàn tương ứng.

- Vấn đề 4: Một số khách của một số đoàn có công việc bận đột xuất nên check-out ngay trong đêm và tiếp tân muốn kiểm tra lại thông tin của các đoàn như ở Vấn đề 2. 
Hãy chỉnh sửa thông tin của các đoàn để chuẩn bị cho cuộc kiểm tra của tiếp tân.

- Vấn đề 5: Một đoàn khách (đã check-in trước đó) muốn check-out cho cả đoàn. 
Hãy thực hiện chỉnh sửa trạng thái của các phòng mà khách đã check-out theo thứ tự và báo cáo trạng thái các phòng của khách sạn lúc này.

*/

#include <stdio.h>
#include <stdlib.h>

#define FLOORS 17
#define ROOMS 12

// Định nghĩa cấu trúc Khánh hàng (tương tự node)
typedef struct GuestType {
    char name; 
    int floor;
    int room;
    struct GuestType* next;
} Guest;

// Định nghĩa cấu trúc Đoàn khách (tương tự linked list)
typedef struct GroupType {
    Guest* leader;
} Group;

// Ma trận trạng thái phòng
int hotel[FLOORS][ROOMS];

// Khởi tạo khách sạn với trạng thái cho trước
void initHotelFromState(int initialState[FLOORS][ROOMS]) {
    for (int i = 0; i < FLOORS; i++) {
        for (int j = 0; j < ROOMS; j++) {
            hotel[i][j] = initialState[i][j];
        }
    }
}

// Hàm in trạng thái các phòng của khách sạn
void printHotel() {
    printf("\n== Hotel Status ==\n");
    for (int i = 0; i < FLOORS; i++) {
        if (i == 0) {
            printf("Ground floor:\t");
        } else {
            printf("Floor %d:\t", i);
        }

        for (int j = 0; j < ROOMS; j++) {
            printf("%d ", hotel[i][j]);
        }

        printf("\n");
    }
}

// Hàm tạo một node Khách mới
Guest* makeGuest(char name, int floor, int room) {
    Guest* newGuest = (Guest*)malloc(sizeof(Guest));
    newGuest->name = name;
    newGuest->floor = floor;
    newGuest->room = room;
    newGuest->next = NULL;
    return newGuest;
}

// Hàm tìm đoàn theo số phòng của trưởng đoàn (trưởng đoàn luôn ở tầng 0)
Group* findGroup(Group* groups, int n, int leaderRoom) {
    for (int i = 0; i < n; i++) {
        if (groups[i].leader != NULL && groups[i].leader->floor == 0 && groups[i].leader->room == leaderRoom) {
            return &groups[i];
        }
    }

    return NULL;
}

// Hàm tìm đoàn theo khách của đoàn
Group* findGroupByGuest(Group* groups, int n, int floor, int room) {
    for (int i = 0; i < n; i++) {
        if (groups[i].leader == NULL) continue;

        Guest* guest = groups[i].leader;
        while (guest != NULL) {
            if (guest->floor == floor && guest->room == room) {
                return &groups[i];
            }
            guest = guest->next;
        }
    }
    return NULL;
}

// Hàm kiểm tra phòng đó của khách lẻ hay khách trong đoàn
int isRoomInGroup(Group* groups, int n, int floorCheck, int roomCheck) {
    for (int i = 0; i < n; i++) {
        if (groups[i].leader == NULL) continue; // Đoàn rỗng thì bỏ qua, kiểm tra đoàn kế tiếp

        Guest* guest = groups[i].leader;
        while (guest != NULL) {
            if (guest->floor == floorCheck && guest->room == roomCheck) {
                return 1;
            }
            guest = guest->next;
        }
        
    }

    return 0;
}
// Hàm check out cho khách lẻ
void singleCheckout(Group* groups, int n, int count) {
    printf("%d single guest(s) checked out:\n", count);

    int checkedOut = 0;
    for (int i = 0; i < FLOORS && checkedOut < count; i++) {
        for (int j = 0; j < ROOMS && checkedOut < count; j++) {
            if (hotel[i][j] == 1 && !isRoomInGroup(groups, n, i, j)) {
                hotel[i][j] = 0;
                printf("- Guest at P%d.%d has checked out!\n", i, j);
                checkedOut++;
            }
        }
    }

    if (count == 1) {
        printf("%d room is empty.\n", count);
    } else {
        printf("%d rooms are empty.\n", count);
    }
}

// VẤN ĐỀ 1: XẾP MỘT ĐOÀN KHÁCH VÀO CÁC PHÒNG TRỐNG
Guest* assignGroup(int nameGroup, int memberCount, char *names) {
    Guest *leader = NULL;
    Guest *prev = NULL;

    for (int m = 0; m < memberCount; m++) {
        int floor, room;
        int found = 0;

        // Trưởng đoàn (m = 0) phải ở tầng trệt
        if (m == 0) {
            for (int j = 0; j < ROOMS && !found; j++) {
                if (hotel[0][j] == 0) {
                    floor = 0;
                    room = j;
                    found = 1;
                }
            }
        } else { // Thành viên khác ở tầng 1 trở lên
            for (int i = 1; i < FLOORS && !found; i++) {
                for (int j = 0; j < ROOMS && !found; j++) {
                    if (hotel[i][j] == 0) {
                        floor = i;
                        room = j;
                        found = 1;
                    }
                }
            }
        }

        if (!found) {
            printf("Hotel is fully booked!\n");
            return leader;
        }

        // Đánh dấu phòng đã có người
        hotel[floor][room] = 1;
        Guest *guest = makeGuest(names[m], floor, room);

        if (m == 0) {
            leader = guest;
        } else {
            prev->next = guest;
        }
        prev = guest;     
    }

    printf("Group %d (leader: %c at P%d.%d) has successfully checked in.\n", nameGroup, leader->name, leader->floor, leader->room);
    return leader;
}

// VẤN ĐỀ 2: In thông tin tất cả các thành viên trong đoàn
void printGroupInfo(Group* group) {
    if (group == NULL || group->leader == NULL) {
        printf("Group is not found!\n");
        return;
    }

    printf("Info of group (leader %c at P%d.%d):\n", group->leader->name, group->leader->floor, group->leader->room);
    Guest* guest = group->leader;
    int count = 1;
    while (guest != NULL) {
        printf("%d. %c - P%d.%d\n", count++, guest->name, guest->floor, guest->room);
        guest = guest->next;
    }
}

// VẤN ĐỀ 3: Thêm khách vào đoàn
void addGuestToGroup(Group* group, int addCount, char *addNames) {
    if (group == NULL || group->leader == NULL) {
        printf("Group is not found!\n");
        return;
    }

    // Tìm khách cuối đoàn
    Guest *last = group->leader;
    while (last->next != NULL) {
        last = last->next;
    }

    // Thêm khách mới vào đoàn
    for (int i = 0; i < addCount; i++) {
        int floor, room, found = 0;

        // Tìm phòng trống
        for (int fi = 0; fi < FLOORS && !found; fi++) {
            for (int ri = 0; ri <ROOMS && !found; ri++) {
                if (hotel[fi][ri] == 0) {
                    floor = fi;
                    room = ri;
                    found = 1;
                }
            }
        }

        if (found) {
            hotel[floor][room] = 1;
            Guest *newGuest = makeGuest(addNames[i], floor, room);
            last->next = newGuest;
            last = newGuest;
            printf("Successfully assigned guest %c of this group to P%d.%d\n", addNames[i], floor, room);
        } else {
            printf("Not found empty room to add new guest!\n");
            break;
        }
    }
}

// VẤN ĐỀ 4: Khách của đoàn check out lẻ
void removeGuestFromGroup(Group* group, int floor, int room) {
    if (group == NULL || group->leader == NULL) {
        printf("Group is not found!\n");
        return;
    }

    Guest* previous = group->leader;
    Guest* current = group->leader->next;
    while (current != NULL) {
        if (current->floor == floor && current->room == room) {
            hotel[floor][room] = 0;
            printf("Guest %c at P%d.%d of group (leader %c at P0.%d) has checked out.\n", current->name, floor, room, group->leader->name, group->leader->room);

            previous->next = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
    
    printf("Not found guest at P%d.%d in this group!\n", floor, room);
}

// VẤN ĐỀ 5: Đoàn khách check out
void groupCheckOut(Group* group) {
    if (group == NULL || group->leader == NULL) {
        printf("Group is not found!\n");
        return;
    }

    printf("Group checked out:\n");

    Guest* guest = group->leader;
    while (guest != NULL) {
        printf(" %c - P%d.%d\n",guest->name, guest->floor, guest->room);
        hotel[guest->floor][guest->room] = 0;
        Guest* temp = guest;
        guest = guest->next;
        free(temp);
    }
    group->leader = NULL;
}

// Hàm giải phóng các đoàn khách
void freeAllGroups(Group* groups, int n) {
    for (int i = 0; i < n; i++) {
        if (groups[i].leader != NULL) {
            Guest* guest = groups[i].leader;
            while (guest != NULL) {
                Guest* temp = guest;
                guest = guest->next;
                free(temp);
            }
            groups[i].leader = NULL;            
        }
    }
}

int main() {
    // Trạng thái ban đầu của khách sạn 
    int initialHotel[FLOORS][ROOMS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // Tầng trệt
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0},
        {1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1},
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1},
        {0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
        {0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1},
        {1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0},
        {1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1},
        {0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
        {0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0},
        {0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, // Tầng 16
    };

    printf("INITIAL STATE OF HOTEL");
    initHotelFromState(initialHotel);
    printHotel();

    int n = 10;
    int members[] = {12, 10, 9, 13, 15, 14, 12, 15, 10, 10};
    char names[10][20] = {
        "ABSEFJKLSWQA",
        "TMKLJSAQCS",
        "HJSSASMQD",
        "SAQDASSAQDSQA",
        "QSAQEDFSSASXVBF",
        "HAVANTHAOTTHUS",
        "PHLAMDSATTHU",
        "PPNHUNGSVELEVEN",
        "LPTRUONGDS",
        "CHGIAOKDLH"
    }; 
    Group groups[10];

    int select;

    int leaderRoom;
    Group *group;

    while (1) {
        printf("\n1. Assign groups to empty rooms\n2. Check info of a group\n3. Add guests to group\n4. Remove guest from group\n5. Check out a group\n6. Exit\nSelect: ");
        scanf("%d", &select);
        getchar();

        if (select == 1) {
            // VẤN ĐỀ 1
            printf("\n************ PROBLEM 1: ASSIGN GROUPS TO EMPTY ROOMS ************\n");
    
            for (int i = 0; i < n; i++) {
                groups[i].leader = assignGroup(i+1, members[i], names[i]);
            }
            
            printHotel();
        }

        else if (select == 2) {
            // VẤN ĐỀ 2
            printf("\n************ PROBLEM 2: CHECK INFO OF A GROUP ************\n");
    
            printf("Enter room of group's leader (from 0 to 11): ");
            scanf("%d", &leaderRoom);
            
            group = findGroup(groups, n, leaderRoom);
            
            printGroupInfo(group);
        }

        else if (select == 3) {
            // VẤN ĐỀ 3
            printf("\n************ PROBLEM 3: ADD GUEST TO GROUP ************\n");
    
            int checkOut;
            printf("Enter number of single guests checking out: ");
            scanf("%d", &checkOut);
            singleCheckout(groups, n, checkOut);

            printf("Enter room of group's leader (from 0 to 11): ");
            scanf("%d", &leaderRoom);
    
            int addCount;
            printf("Enter the number of new guests to add: ");
            scanf("%d", &addCount);

            char addNames[10];
            printf("Enter guest's name: ");
            for (int i = 0; i < addCount; i++) {
                scanf(" %c", &addNames[i]);
            }

            group = findGroup(groups, n, leaderRoom);
            addGuestToGroup(group, addCount, addNames);
            printGroupInfo(group);
        }

        else if (select == 4) {
            // VẤN ĐỀ 4
            printf("\n************ PROBLEM 4: REMOVE GUEST FROM GROUP ************\n");
    
            int outCount;
    
            printf("Enter number of guests checking out: ");
            scanf("%d", &outCount);

            for (int i = 0; i < outCount; i++) {
                printf("Guest %d - Enter floor & room: ", i + 1);
                int floor, room;
                scanf("%d %d", &floor, &room);
                group = findGroupByGuest(groups, n, floor, room);
                removeGuestFromGroup(group, floor, room);
            }

            printHotel();
        }

        else if (select == 5) {
            // VẤN ĐỀ 5
            printf("\n************ PROBLEM 5: GROUP CHECK OUT ************\n");
    
            printf("Enter room of group's leader (from 0 to 11): ");
            scanf("%d", &leaderRoom);

            group = findGroup(groups, n, leaderRoom);
            groupCheckOut(group);
            printHotel();
        }

        else break;
    } 

    // KẾT THÚC
    freeAllGroups(groups, n);
    return 0;
}

