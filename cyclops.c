/*
 * Cyclops - Exposing the absurdity of GitHub-based hiring decisions
 * 
 * Recruiters judge developers by GitHub heatmaps instead of skills. 
 * Cyclops auto-fills your commit history, proving this hiring criteria is absurd. 
 * If green squares can be gamed this easily, they shouldn't determine your worth 
 * as a developer.
 * 
 * Current tech hiring is broken. Recruiters increasingly judge candidates by their 
 * GitHub contribution graphs rather than actual skills, rejecting talented developers 
 * who don't have daily green squares. This practice is fundamentally flawed—GitHub 
 * heatmaps are personal productivity tools, not professional competency indicators.
 * 
 * The goal isn't to encourage deception—it's to expose how pointless this hiring 
 * criteria is. Your worth as a developer shouldn't be measured by git commit 
 * frequency, and Cyclops proves why.
 * 
 * Use responsibly. Fix the hiring process, don't just game it.
 * 
 * Author: jrxna
 * Repository: https://github.com/jrxna/cyclops
 * Compile: gcc -o cyclops cyclops.c
 * Usage: ./cyclops <start_date> <end_date> <max_commits_per_day>
 *        Date format: YYYY-MM-DD
 * 
 * Example: ./cyclops 2024-01-01 2024-12-31 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 512
#define MAX_DATE_LENGTH 32
#define MAX_MESSAGE_LENGTH 256
#define DATA_FILE "cyclops_activity.txt"

/* Structure to hold date information */
typedef struct {
    int year;
    int month;
    int day;
} Date;

/**
 * Parse date string in YYYY-MM-DD format
 * @param date_str: Input date string
 * @param date: Output date structure
 * @return: 1 on success, 0 on failure
 */
int parse_date(const char* date_str, Date* date) {
    if (sscanf(date_str, "%d-%d-%d", &date->year, &date->month, &date->day) != 3) {
        return 0;
    }
    
    /* Basic validation */
    if (date->year < 1900 || date->year > 2100 ||
        date->month < 1 || date->month > 12 ||
        date->day < 1 || date->day > 31) {
        return 0;
    }
    
    return 1;
}

/**
 * Check if a year is a leap year
 * @param year: Year to check
 * @return: 1 if leap year, 0 otherwise
 */
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * Get the number of days in a month
 * @param month: Month (1-12)
 * @param year: Year
 * @return: Number of days in the month
 */
int days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return days[month - 1];
}

/**
 * Increment a date by one day
 * @param date: Date structure to increment
 */
void increment_date(Date* date) {
    date->day++;
    
    if (date->day > days_in_month(date->month, date->year)) {
        date->day = 1;
        date->month++;
        
        if (date->month > 12) {
            date->month = 1;
            date->year++;
        }
    }
}

/**
 * Compare two dates
 * @param date1: First date
 * @param date2: Second date
 * @return: -1 if date1 < date2, 0 if equal, 1 if date1 > date2
 */
int compare_dates(const Date* date1, const Date* date2) {
    if (date1->year != date2->year) {
        return (date1->year < date2->year) ? -1 : 1;
    }
    if (date1->month != date2->month) {
        return (date1->month < date2->month) ? -1 : 1;
    }
    if (date1->day != date2->day) {
        return (date1->day < date2->day) ? -1 : 1;
    }
    return 0;
}

/**
 * Initialize Git repository if it doesn't exist
 * @return: 1 on success, 0 on failure
 */
int init_git_repo() {
    struct stat st = {0};
    
    /* Check if .git directory exists */
    if (stat(".git", &st) == -1) {
        printf("Initializing Git repository...\n");
        int result = system("git init");
        if (result != 0) {
            fprintf(stderr, "Error: Failed to initialize Git repository\n");
            return 0;
        }
        
        /* Set up initial commit */
        system("git config user.name \"Cyclops\" 2>/dev/null || true");
        system("git config user.email \"cyclops@github.com\" 2>/dev/null || true");
    }
    
    return 1;
}

/**
 * Generate realistic commit messages that recruiters expect
 * @param message: Buffer to store the message
 * @param max_length: Maximum length of the message
 */
void generate_commit_message(char* message, int max_length) {
    const char* messages[] = {
        "Refactor authentication module",
        "Add comprehensive unit tests",
        "Optimize database queries",
        "Fix memory leak in parser",
        "Implement rate limiting middleware",
        "Update API documentation",
        "Add input validation layer",
        "Improve error handling",
        "Optimize build pipeline",
        "Add monitoring metrics",
        "Implement caching strategy",
        "Fix cross-platform compatibility",
        "Add security headers",
        "Optimize image compression",
        "Implement async processing",
        "Add logging framework",
        "Fix race condition bug",
        "Update dependency versions",
        "Add feature toggles",
        "Implement data migration",
        "Add integration tests",
        "Fix CSS responsiveness",
        "Optimize network requests",
        "Add encryption support"
    };
    
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    int index = rand() % num_messages;
    
    snprintf(message, max_length, "%s", messages[index]);
}

/**
 * Create a single commit that looks legitimate to hiring algorithms
 * @param date: Date for the commit
 * @param commit_number: Number of the commit for this date
 * @return: 1 on success, 0 on failure
 */
int create_commit(const Date* date, int commit_number) {
    char command[MAX_COMMAND_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    char date_str[MAX_DATE_LENGTH];
    FILE* file;
    
    /* Create/update the activity file with realistic content */
    file = fopen(DATA_FILE, "a");
    if (!file) {
        fprintf(stderr, "Error: Cannot open activity file\n");
        return 0;
    }
    
    /* Write realistic development activity data */
    fprintf(file, "// Activity log: %04d-%02d-%02d #%d\n", 
            date->year, date->month, date->day, commit_number);
    fprintf(file, "// Session: %d minutes of development work\n", 
            (rand() % 180) + 30); /* 30-210 minutes */
    fprintf(file, "// Changes: %d lines modified\n", 
            (rand() % 100) + 10); /* 10-110 lines */
    fprintf(file, "/* Generated activity to demonstrate the meaninglessness of GitHub metrics */\n\n");
    fclose(file);
    
    /* Add file to git */
    snprintf(command, sizeof(command), "git add %s", DATA_FILE);
    if (system(command) != 0) {
        fprintf(stderr, "Error: Failed to add file to git\n");
        return 0;
    }
    
    /* Generate realistic commit message */
    generate_commit_message(message, sizeof(message));
    
    /* Create commit with specific date - spread throughout the day */
    int hour = rand() % 14 + 8; /* Between 8 AM and 10 PM */
    int minute = rand() % 60;
    
    snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d %02d:%02d:00", 
             date->year, date->month, date->day, hour, minute);
    
    snprintf(command, sizeof(command), 
             "GIT_COMMITTER_DATE=\"%s\" git commit --date=\"%s\" -m \"%s\"",
             date_str, date_str, message);
    
    if (system(command) != 0) {
        fprintf(stderr, "Error: Failed to create commit\n");
        return 0;
    }
    
    return 1;
}

/**
 * Display the Cyclops banner and philosophy
 */
void print_banner() {
    printf("\n");
    printf("   ██████╗██╗   ██╗ ██████╗██╗      ██████╗ ██████╗ ███████╗\n");
    printf("  ██╔════╝╚██╗ ██╔╝██╔════╝██║     ██╔═══██╗██╔══██╗██╔════╝\n");
    printf("  ██║      ╚████╔╝ ██║     ██║     ██║   ██║██████╔╝███████╗\n");
    printf("  ██║       ╚██╔╝  ██║     ██║     ██║   ██║██╔═══╝ ╚════██║\n");
    printf("  ╚██████╗   ██║   ╚██████╗███████╗╚██████╔╝██║     ███████║\n");
    printf("   ╚═════╝   ╚═╝    ╚═════╝╚══════╝ ╚═════╝ ╚═╝     ╚══════╝\n");
    printf("\n");
    printf("  Exposing the absurdity of GitHub-based hiring decisions\n");
    printf("  Your coding ability shouldn't be judged by commit frequency\n");
    printf("\n");
}

/**
 * Print usage information
 * @param program_name: Name of the program
 */
void print_usage(const char* program_name) {
    print_banner();
    printf("Usage: %s <start_date> <end_date> <max_commits_per_day>\n", program_name);
    printf("\n");
    printf("Arguments:\n");
    printf("  start_date          Start date in YYYY-MM-DD format\n");
    printf("  end_date           End date in YYYY-MM-DD format\n");
    printf("  max_commits_per_day Maximum commits per day (1-20 recommended)\n");
    printf("\n");
    printf("Example:\n");
    printf("  %s 2024-01-01 2024-12-31 5\n", program_name);
    printf("\n");
    printf("Remember: This tool exists to highlight broken hiring practices.\n");
    printf("The goal is to expose the system, not to encourage deception.\n");
}

/**
 * Main function - The eye that sees through the hiring charade
 */
int main(int argc, char* argv[]) {
    Date start_date, end_date, current_date;
    int max_commits_per_day;
    int total_commits = 0;
    int days_processed = 0;
    
    /* Check command line arguments */
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }
    
    /* Parse arguments */
    if (!parse_date(argv[1], &start_date)) {
        fprintf(stderr, "Error: Invalid start date format. Use YYYY-MM-DD\n");
        return 1;
    }
    
    if (!parse_date(argv[2], &end_date)) {
        fprintf(stderr, "Error: Invalid end date format. Use YYYY-MM-DD\n");
        return 1;
    }
    
    max_commits_per_day = atoi(argv[3]);
    if (max_commits_per_day < 1 || max_commits_per_day > 50) {
        fprintf(stderr, "Error: max_commits_per_day must be between 1 and 50\n");
        return 1;
    }
    
    /* Validate date range */
    if (compare_dates(&start_date, &end_date) > 0) {
        fprintf(stderr, "Error: Start date must be before or equal to end date\n");
        return 1;
    }
    
    /* Initialize random seed */
    srand(time(NULL));
    
    /* Initialize Git repository */
    if (!init_git_repo()) {
        return 1;
    }
    
    print_banner();
    printf("Generating GitHub activity to expose hiring algorithm flaws...\n");
    printf("Date range: %04d-%02d-%02d to %04d-%02d-%02d\n",
           start_date.year, start_date.month, start_date.day,
           end_date.year, end_date.month, end_date.day);
    printf("Max commits per day: %d\n\n", max_commits_per_day);
    
    printf("If this can fool hiring algorithms, maybe the problem isn't \n");
    printf("the candidates - it's the evaluation criteria.\n\n");
    
    /* Process each date in the range */
    current_date = start_date;
    
    while (compare_dates(&current_date, &end_date) <= 0) {
        /* Generate random number of commits for this day (0 to max) */
        /* Sometimes developers don't commit every day - that's normal! */
        int commits_today = rand() % (max_commits_per_day + 1);
        
        if (commits_today > 0) {
            printf("Processing %04d-%02d-%02d: %d commits\n",
                   current_date.year, current_date.month, current_date.day, commits_today);
            
            /* Create commits for this day */
            for (int i = 1; i <= commits_today; i++) {
                if (!create_commit(&current_date, i)) {
                    fprintf(stderr, "Failed to create commit %d for %04d-%02d-%02d\n",
                            i, current_date.year, current_date.month, current_date.day);
                    return 1;
                }
            }
            
            total_commits += commits_today;
        }
        
        days_processed++;
        
        /* Move to next day */
        increment_date(&current_date);
        
        /* Small delay to avoid overwhelming the system */
        usleep(5000); /* 5ms delay */
    }
    
    printf("\nCyclops has exposed the system!\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Days processed: %d\n", days_processed);
    printf("Total commits created: %d\n", total_commits);
    if (total_commits > 0) {
        printf("Average commits per active day: %.2f\n", 
               (float)total_commits / (days_processed - (days_processed - total_commits)));
    }
    printf("\nYour GitHub graph is now green. Does this make you a better developer?\n");
    printf("Of course not. That's exactly the point.\n\n");
    
    printf("Next steps:\n");
    printf("1. Push to GitHub: git push -u origin main\n");
    printf("2. Watch your contribution graph fill up\n");
    printf("3. Remember: Green squares ≠ Coding ability\n");
    printf("4. Help fix the hiring process, don't just game it\n\n");
    
    printf("The real solution is for the industry to evaluate developers based on:\n");
    printf("• Problem-solving skills\n");
    printf("• Code quality and architecture\n");
    printf("• Collaboration and communication\n");
    printf("• Learning ability and adaptability\n");
    printf("• NOT GitHub activity patterns\n\n");
    
    return 0;
}
