#include <stdio.h>
#include <stdlib.h>

#define MAX_MEETINGS 100

typedef struct {
    int start;
    int end;
} Meeting;

Meeting meetings[MAX_MEETINGS];
int meetingCount = 0;

// Function to compare two meetings by their start time
int compare(const void* a, const void* b) {
    Meeting* m1 = (Meeting*)a;
    Meeting* m2 = (Meeting*)b;
    return m1->start - m2->start;
}

// Function to check if two meetings overlap
int isOverlapping(int start1, int end1, int start2, int end2) {
    return (start1 < end2 && start2 < end1);
}

// Function to book a meeting
void bookMeeting(int start, int end) {
    for (int i = 0; i < meetingCount; i++) {
        if (isOverlapping(start, end, meetings[i].start, meetings[i].end)) {
            printf("NA\n");
            return;
        }
    }
    
    meetings[meetingCount].start = start;
    meetings[meetingCount].end = end;
    meetingCount++;
    printf("Booked [%d %d]\n", start, end);
    
    // Sort meetings after booking
    qsort(meetings, meetingCount, sizeof(Meeting), compare);
}

// Function to cancel a meeting
void cancelMeeting(int start, int end) {
    int found = 0;
    for (int i = 0; i < meetingCount; i++) {
        if (meetings[i].start == start && meetings[i].end == end) {
            found = 1;
            for (int j = i; j < meetingCount - 1; j++) {
                meetings[j] = meetings[j + 1];
            }
            meetingCount--;
            printf("Canceled [%d %d]\n", start, end);
            break;
        }
    }
    if (!found) {
        printf("NA\n");
    }
}

// Function to list all currently booked meetings
void listMeetings() {
    if (meetingCount == 0) {
        printf("NA\n");
    } else {
        for (int i = 0; i < meetingCount; i++) {
            printf("[%d %d] ", meetings[i].start, meetings[i].end);
        }
        printf("\n");
    }
}

int main() {
    int n;
    scanf("%d", &n); // Number of operations
    
    for (int i = 0; i < n; i++) {
        char operation;
        int start, end;
        scanf(" %c", &operation); // Read operation (B, C, or L)
        
        if (operation == 'B') {
            scanf("%d %d", &start, &end);
            bookMeeting(start, end);
        } else if (operation == 'C') {
            scanf("%d %d", &start, &end);
            cancelMeeting(start, end);
        } else if (operation == 'L') {
            listMeetings();
        }
    }
    
    return 0;
}
