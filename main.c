#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *pick_quote(const char *path) {
    FILE *qf = fopen(path, "r");
    // if file cant be opened return NULL
    if (!qf) 
    return NULL;

    int count = 0;
    char tmp[512];
    while (fgets(tmp, sizeof tmp, qf)) 
    count++;
    rewind(qf);   // reset fp

    if (!count) { 
    fclose(qf); 
    return NULL; 
    }

    int target = rand() % count;
    static char buf[512];
    for (int i = 0; i <= target && fgets(buf, sizeof buf, qf); i++);
    buf[strcspn(buf, "\n")] = '\0';   // remove new line charachter
    fclose(qf);
    return buf;
}

int main(void) {
    srand(time(NULL));

    while (1){
    FILE *f = fopen("/proc/meminfo", "r"); //opening file as read mode
    if (!f)
    return 1;

    unsigned long total_kb = 0, avail_kb = 0;
    char line[256];
    while (fgets(line, sizeof line, f)) {
        sscanf(line, "MemTotal: %lu kB", &total_kb);
        sscanf(line, "MemAvailable: %lu kB", &avail_kb);
    }
    fclose(f);

    // Converting kb to gb
    unsigned long used_kb = total_kb - avail_kb;
    double total_gib = total_kb / 1048576.0;
    double used_gib  = used_kb  / 1048576.0;
    int percent = (int)(used_kb * 100.0 / total_kb);

    const char *quote =
        pick_quote(percent < 30 ? "terrible/low.txt" :
                   percent < 70 ? "terrible/normal.txt" :
                                  "terrible/high.txt");
    system("clear");
    printf("RAM: %.1f / %.1f GiB (%d%% used) %s\n", used_gib, total_gib, percent, quote);
    sleep(5);
    }
    return 0;
}
