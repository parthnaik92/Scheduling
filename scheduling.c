/*ID 201001185 - Parth Naik*/
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
/* Process Data Structure */
int n[1024], a[5], b[5], c[5];
char filename[20];
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
int string_read=FALSE, i;
FILE *fp;
pthread_cond_t cond;
void * read1()
{
while(1)
{
while(string_read);
pthread_mutex_lock(&lock);
printf("Input file name with extension:\n ");
scanf("%s",filename);
fp=fopen(filename, "w");
printf("Input data\n");
printf("PID Brusttime Priority\n");
for(i=0 ; i<5 ; i++)
{
fscanf(stdin,"%d %d %d", &a[i], &b[i], &c[i]);
printf("\n");
fprintf(fp,"%d %d %d\n", a[i], b[i], c[i]);
}
fclose(fp);
string_read=TRUE;
pthread_mutex_unlock(&lock);
pthread_cond_signal(&cond);

pthread_exit(NULL);
}
}
void * write1()
{
while(1){
pthread_mutex_lock(&lock);
while(!string_read)
pthread_cond_wait(&cond,&lock);
fprintf(stdout, "\n\n");
fp= fopen(filename, "r");
printf("PID Brusttime Priority\n");
for(i=0; i<5; i++)
{
fscanf(fp, "%d %d %d",&a[i], &b[i], &c[i]);
printf("Process:");
fprintf(stdout, "%d %d %d \n", a[i], b[i], c[i]);
}
fclose(fp);
string_read=FALSE;
pthread_mutex_unlock(&lock);
pthread_exit(NULL);
}
}
struct process {
int pid;
int burst;
int priority;
int working; /*rr scheduling */
int waiting; /*rr scheduling */
struct process *next;
};
struct process *init_process (int pid, int burst, int priority);
void fcfs (struct process *proc);
void listprocs (struct process *proc);
//void priority (struct process *proc);
void rr (struct process *proc, int quantum);
void sjf (struct process *proc);
void display(struct process *a);
int main (void) {
struct process *plist, *ptmp;
pthread_t tr, tw;
pthread_create(&tr,NULL,read1,NULL);
pthread_create(&tw,NULL,write1,NULL);
pthread_join(tr,NULL);
pthread_join(tw,NULL);
plist = init_process(a[0], b[0], c[0]);
plist->next = init_process(a[1], b[1], c[1]); ptmp = plist->next;

ptmp->next = init_process(a[2], b[2], c[2]); ptmp = ptmp->next;
ptmp->next = init_process(a[3], b[3], c[3]); ptmp = ptmp->next;
ptmp->next = init_process(a[4], b[4], c[4]);
display(plist);
while (plist != NULL) {
ptmp = plist;
plist = plist->next;
free(ptmp);
};
return(0);
};
struct process *init_process (int pid, int burst, int priority) {
struct process *proc;
proc = malloc(sizeof(struct process));
if (proc == NULL) {
printf("Fatal error: memory allocation failure.\nTerminating.\n");
exit(1);
};
proc->pid = pid;
proc->burst = burst;
proc->priority = priority;
proc->working = 0;
proc->waiting = 0;
proc->next = NULL;
return(proc);
};
void display(struct process *a){
printf("Please enter your choice\n");//\n 1 for list of process
printf("\n 1 for list of process\n 2 for fcfs\n 3 for Round Robin\n 4 for Shortest job first algo\n  5 for display all\n 6 for exit\n-->");
int aa;
scanf("%d",&aa);
switch(aa){
case 1:listprocs(a);
display(a);
break;
case 2:fcfs(a);
display(a);
break;
case 3:rr(a, 1);
display(a);
break;
case 4:sjf(a);
display(a);
break;

case 5:listprocs(a);

fcfs(a);
rr(a, 1);
sjf(a);
display(a); break;
case 6:
break;
default:
display(a); break;
}
}
/* FCFS scheduling algo */
void fcfs (struct process *proc) {
int time = 0, start, end;
struct process *tmp = proc;
printf("BEGIN:\tFCFS scheduling algo\n");
while (tmp != NULL) {
start = time;
time += tmp->burst;
end = time;
printf("Process: %d\tEnd Time: %d\tWaiting: %d\tTurnaround: %d\n", tmp->pid, time, start, end);
tmp = tmp->next;
};
printf("END:\tFCFS scheduling algo\n\n");
};
void listprocs (struct process *proc) {
struct process *tmp = proc;
printf("BEGIN:\tProcess Listing\n");
while (tmp != NULL) {
printf("PID: %d\t\tPriority: %d\tBurst: %d\n", tmp->pid, tmp->priority, tmp->burst);
tmp = tmp->next;
};
printf("END:\tProcess Listing\n\n");
};
/* RR scheduling algo */
void rr (struct process *proc, int quantum) {
int jobsremain, passes;
struct process *copy, *tmpsrc, *tmp, *slot;
printf("BEGIN:\tRR scheduling algo (Quantum: %d)\n", quantum);
/* Duplicate process list */
tmpsrc = proc;
copy = tmp = NULL;
while (tmpsrc != NULL) {
if (copy == NULL) {
copy = init_process(tmpsrc->pid, tmpsrc->burst, tmpsrc->priority);
tmp = copy;
} else {
tmp->next = init_process(tmpsrc->pid, tmpsrc->burst, tmpsrc->priority);
tmp = tmp->next;
};
tmpsrc = tmpsrc->next;
};
/* Main routine */
jobsremain = 1;
slot = NULL;
while (jobsremain) {
jobsremain = 0;
/* Pick next working slot */
if (slot == NULL) {
slot = copy;
jobsremain = 1;
} else {
passes = 0;
do {
if (slot->next == NULL) {
passes++;
slot = copy;
} else {
slot = slot->next;
};
} while (passes <= 2 && slot->burst == slot->working);
if (passes <= 2) {
jobsremain = 1;
};
};
/* Perform a cycle */
tmp = copy;
while (tmp != NULL) {
if (tmp->burst > tmp->working) {
if (tmp == slot) {
tmp->working += quantum;

} else {
tmp->waiting += quantum;
};
};
tmp = tmp->next;
};
};
/* Display statistics and clean up copy */
tmp = copy;
while (tmp != NULL) {
printf("Process: %d\tWorking: %d\tWaiting: %d\tTurnaround: %d\n", tmp->pid, tmp->working, tmp->waiting, tmp->working + tmp->waiting);
tmpsrc = tmp;
tmp = tmp->next;
free(tmpsrc);
};
printf("END:\tRR scheduling algo\n\n");
};
/* SJF scheduling algo */
void sjf (struct process *proc) {
int time, start, end, shortest;
struct process *copy, *tmpsrc, *tmp, *beforeshortest;
printf("BEGIN:\tSJF scheduling algo\n");
/* Duplicate process list */
tmpsrc = proc;
copy = tmp = NULL;
while (tmpsrc != NULL) {
if (copy == NULL) {
copy = init_process(tmpsrc->pid, tmpsrc->burst, tmpsrc->priority);
tmp = copy;
} else {
tmp->next = init_process(tmpsrc->pid, tmpsrc->burst, tmpsrc->priority);
tmp = tmp->next;
};
tmpsrc = tmpsrc->next;
};
/* Main routine */
time = 0;
while (copy != NULL) {
/* Find the next job */
beforeshortest = NULL;
shortest = copy->burst;
tmp = copy->next;
tmpsrc = copy;
while (tmp != NULL) {
if (tmp->burst < shortest) {
shortest = tmp->burst;
beforeshortest = tmpsrc;
};

tmpsrc = tmp;
tmp = tmp->next;
};
/* Process job and remove from copy of process list */
if (beforeshortest == NULL) {
/* Handle first job is shortest case */
start = time;
time += copy->burst;
end = time;
printf("Process: %d\tEnd Time: %d\tWaiting: %d\tTurnaround: %d\n", copy->pid, time, start, end);
tmpsrc = copy;
copy = copy->next;
free(tmpsrc);
} else {
/* Handle first job is not shortest case */
tmp = beforeshortest->next;
start = time;
time += tmp->burst;
end = time;
printf("Process: %d\tEnd Time: %d\tWaiting: %d\tTurnaround: %d\n", tmp->pid, time, start, end);
beforeshortest->next = tmp->next;
free(tmp);
};
};
printf("END:\tSJF scheduling algo\n\n");
};
