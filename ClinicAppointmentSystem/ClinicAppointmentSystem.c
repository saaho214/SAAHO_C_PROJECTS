#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =============================================================
// Clinic Appointment System (Arrays, Admin/User, Auth)
// - Admin & Patient (User) modes with authentication
// - Arrays (no file I/O), menu-driven
// - Doctor management (CRUD), Treatment management (CRUD)
// - Appointment booking, approval/rejection, reschedule, cancel
// - Validation: double booking prevention, doctor x treatment
// - Tabular output formatting, simple reports
// =============================================================

#define MAX_DOCTORS     100
#define MAX_TREATMENTS  100
#define MAX_PATIENTS    200
#define MAX_APPTS       1000
#define STR_SMALL       32
#define STR_MED         64
#define STR_LARGE       128

// ----------------------------- Data Types

typedef struct {
    int id;
    char name[STR_MED];
    char specialization[STR_MED];
    char timeFrom[STR_SMALL];   // e.g., "09:00"
    char timeTo[STR_SMALL];     // e.g., "17:00"
    int active;                 // 1=active,0=deleted
} Doctor;

typedef struct {
    int id;
    char name[STR_MED];
    int durationMin;            // 15/30/60...
    float cost;                 // simple number
    int active;                 // 1=active,0=deleted
} Treatment;

typedef struct {
    int id;                     // patient id (login id)
    char name[STR_MED];
    int age;
    char gender[STR_SMALL];
    char phone[STR_SMALL];
    char password[STR_SMALL];
    int active;                 // 1=active,0=deleted
} Patient;

typedef struct {
    int id;                     // auto increment
    int patientId;
    int doctorId;
    int treatmentId;
    char date[STR_SMALL];       // DD-MM-YYYY
    char time[STR_SMALL];       // HH:MM (start time)
    char status[STR_SMALL];     // Pending/Approved/Rejected/Cancelled
    int active;                 // 1=active,0=deleted
} Appointment;

// ----------------------------- Storage (Arrays)

Doctor doctors[MAX_DOCTORS];            int doctorCount=0;
Treatment treatments[MAX_TREATMENTS];   int treatmentCount=0;
Patient patients[MAX_PATIENTS];         int patientCount=0;
Appointment appts[MAX_APPTS];           int apptCount=0; int NEXT_APPT_ID=1;

int doctorTreats[MAX_DOCTORS][MAX_TREATMENTS];

const char *ADMIN_USER = "admin";
const char *ADMIN_PASS = "admin";

// ----------------------------- Helpers

void pauseEnter(){ printf("\nPress ENTER to continue..."); int c; while((c=getchar())!='\n' && c!=EOF){} }

int findDoctorIndexById(int id){ for(int i=0;i<doctorCount;i++) if(doctors[i].active && doctors[i].id==id) return i; return -1; }
int findTreatmentIndexById(int id){ for(int i=0;i<treatmentCount;i++) if(treatments[i].active && treatments[i].id==id) return i; return -1; }
int findPatientIndexById(int id){ for(int i=0;i<patientCount;i++) if(patients[i].active && patients[i].id==id) return i; return -1; }

int idExistsDoctor(int id){ return findDoctorIndexById(id)>=0; }
int idExistsTreatment(int id){ return findTreatmentIndexById(id)>=0; }
int idExistsPatient(int id){ return findPatientIndexById(id)>=0; }

int timeEq(const char *a,const char *b){ return strcmp(a,b)==0; }
int dateEq(const char *a,const char *b){ return strcmp(a,b)==0; }

int isSlotBusyDoctor(int doctorId, const char *date, const char *time){
    for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].doctorId==doctorId && dateEq(appts[i].date,date) && timeEq(appts[i].time,time) && strcmp(appts[i].status,"Rejected")!=0 && strcmp(appts[i].status,"Cancelled")!=0) return 1; return 0; }

int isSlotBusyPatient(int patientId, const char *date, const char *time){
    for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].patientId==patientId && dateEq(appts[i].date,date) && timeEq(appts[i].time,time) && strcmp(appts[i].status,"Rejected")!=0 && strcmp(appts[i].status,"Cancelled")!=0) return 1; return 0; }

void printLine(int n){ for(int i=0;i<n;i++) putchar('-'); putchar('\n'); }

// ----------------------------- Seed (optional)
void seedDemo(){
    // Doctors
    doctors[doctorCount++] = (Doctor){101,"Dr. saaho","Cardiologist","10:00","14:00",1};
    doctors[doctorCount++] = (Doctor){102,"Dr. vijay","Dentist","12:00","17:00",1};
    doctors[doctorCount++] = (Doctor){103,"Dr. chirag","Dermatologist","09:00","15:00",1};

    // Treatments
    treatments[treatmentCount++] = (Treatment){201,"Consultation",15,500.0f,1};
    treatments[treatmentCount++] = (Treatment){202,"Tooth Extraction",45,2000.0f,1};
    treatments[treatmentCount++] = (Treatment){203,"Skin Check",20,800.0f,1};

    // Doctor ↔ Treatment mapping
    // Dr. Smith: Consultation
    doctorTreats[0][0] = 1;
    // Dr. Patel: Consultation + Tooth Extraction
    doctorTreats[1][0] = 1; doctorTreats[1][1] = 1;
    // Dr. Chen: Consultation + Skin Check
    doctorTreats[2][0] = 1; doctorTreats[2][2] = 1;

    // Patients
    patients[patientCount++] = (Patient){301,"Alice",28,"F","99999","alice",1};
    patients[patientCount++] = (Patient){302,"Bob",34,"M","88888","bob",1};
}

// ----------------------------- Tabular Printers

void listDoctors(){
    printLine(68);
    printf("%-6s %-20s %-18s %-8s %-8s\n","ID","Name","Specialization","From","To");
    printLine(68);
    for(int i=0;i<doctorCount;i++) if(doctors[i].active)
        printf("%-6d %-20s %-18s %-8s %-8s\n",doctors[i].id,doctors[i].name,doctors[i].specialization,doctors[i].timeFrom,doctors[i].timeTo);
    printLine(68);
}

void listTreatments(){
    printLine(64);
    printf("%-6s %-22s %-12s %-10s\n","ID","Treatment","Duration","Cost");
    printLine(64);
    for(int i=0;i<treatmentCount;i++) if(treatments[i].active)
        printf("%-6d %-22s %-12d %-10.2f\n",treatments[i].id,treatments[i].name,treatments[i].durationMin,treatments[i].cost);
    printLine(64);
}

void listPatients(){
    printLine(72);
    printf("%-6s %-18s %-4s %-8s %-14s\n","ID","Name","Age","Gender","Phone");
    printLine(72);
    for(int i=0;i<patientCount;i++) if(patients[i].active)
        printf("%-6d %-18s %-4d %-8s %-14s\n",patients[i].id,patients[i].name,patients[i].age,patients[i].gender,patients[i].phone);
    printLine(72);
}

void listAppointments(int onlyForPatientId, int onlyForDoctorId){
    printLine(90);
    printf("%-5s %-7s %-7s %-8s %-12s %-8s %-12s\n","AID","PatID","DocID","TrtID","Date","Time","Status");
    printLine(90);
    for(int i=0;i<apptCount;i++) if(appts[i].active){
        if(onlyForPatientId>0 && appts[i].patientId!=onlyForPatientId) continue;
        if(onlyForDoctorId>0 && appts[i].doctorId!=onlyForDoctorId) continue;
        printf("%-5d %-7d %-7d %-8d %-12s %-8s %-12s\n", appts[i].id, appts[i].patientId, appts[i].doctorId, appts[i].treatmentId, appts[i].date, appts[i].time, appts[i].status);
    }
    printLine(90);
}

// ----------------------------- Admin: Doctor CRUD

void adminAddDoctor(){
    if(doctorCount>=MAX_DOCTORS){ printf("Doctor list full.\n"); return; }
    Doctor d; memset(&d,0,sizeof(d));
    printf("New Doctor ID: "); scanf("%d", &d.id);
    if(idExistsDoctor(d.id)){ printf("ID already exists.\n"); return; }
    printf("Name: "); getchar(); fgets(d.name, STR_MED, stdin); d.name[strcspn(d.name,"\n")] = 0;
    printf("Specialization: "); fgets(d.specialization, STR_MED, stdin); d.specialization[strcspn(d.specialization,"\n")] = 0;
    printf("Available From (HH:MM): "); scanf("%s", d.timeFrom);
    printf("Available To   (HH:MM): "); scanf("%s", d.timeTo);
    d.active=1;
    doctors[doctorCount++] = d;
    printf("Doctor added.\n");
}

void adminUpdateDoctor(){
    int id; printf("Doctor ID to update: "); scanf("%d", &id);
    int idx = findDoctorIndexById(id); if(idx<0){ printf("Not found.\n"); return; }
    Doctor *d = &doctors[idx]; getchar();
    char buf[STR_MED];
    printf("Name (%s): ", d->name); fgets(buf, STR_MED, stdin); if(buf[0]!='\n'){ buf[strcspn(buf,"\n")] = 0; strncpy(d->name, buf, STR_MED); }
    printf("Specialization (%s): ", d->specialization); fgets(buf, STR_MED, stdin); if(buf[0]!='\n'){ buf[strcspn(buf,"\n")] = 0; strncpy(d->specialization, buf, STR_MED); }
    printf("From (%s): ", d->timeFrom); scanf("%s", buf); if(strlen(buf)) strncpy(d->timeFrom, buf, STR_SMALL);
    printf("To   (%s): ", d->timeTo); scanf("%s", buf); if(strlen(buf)) strncpy(d->timeTo, buf, STR_SMALL);
    printf("Updated.\n");
}

void adminDeleteDoctor(){
    int id; printf("Doctor ID to delete: "); scanf("%d", &id);
    int idx = findDoctorIndexById(id); if(idx<0){ printf("Not found.\n"); return; }
    doctors[idx].active=0; 
    // Also clear mappings
    for(int t=0;t<treatmentCount;t++) doctorTreats[idx][t]=0;
    printf("Doctor deleted (soft).\n");
}

void adminMapTreatmentToDoctor(){
    int did, tid; printf("Doctor ID: "); scanf("%d", &did); printf("Treatment ID: "); scanf("%d", &tid);
    int dIdx = findDoctorIndexById(did); if(dIdx<0){ printf("Doctor not found.\n"); return; }
    int tIdx = findTreatmentIndexById(tid); if(tIdx<0){ printf("Treatment not found.\n"); return; }
    doctorTreats[dIdx][tIdx] = 1; printf("Mapped.\n");
}

void adminUnmapTreatmentFromDoctor(){
    int did, tid; printf("Doctor ID: "); scanf("%d", &did); printf("Treatment ID: "); scanf("%d", &tid);
    int dIdx = findDoctorIndexById(did); if(dIdx<0){ printf("Doctor not found.\n"); return; }
    int tIdx = findTreatmentIndexById(tid); if(tIdx<0){ printf("Treatment not found.\n"); return; }
    doctorTreats[dIdx][tIdx] = 0; printf("Unmapped.\n");
}

void searchDoctorBySpecialization(){
    char spec[STR_MED]; getchar(); printf("Specialization to search: "); fgets(spec, STR_MED, stdin); spec[strcspn(spec,"\n")] = 0;
    printLine(68);
    printf("%-6s %-20s %-18s %-8s %-8s\n","ID","Name","Specialization","From","To");
    printLine(68);
    for(int i=0;i<doctorCount;i++) if(doctors[i].active && strcasecmp(doctors[i].specialization, spec)==0)
        printf("%-6d %-20s %-18s %-8s %-8s\n",doctors[i].id,doctors[i].name,doctors[i].specialization,doctors[i].timeFrom,doctors[i].timeTo);
    printLine(68);
}

// ----------------------------- Admin: Treatment CRUD

void adminAddTreatment(){
    if(treatmentCount>=MAX_TREATMENTS){ printf("Treatment list full.\n"); return; }
    Treatment t; memset(&t,0,sizeof(t));
    printf("New Treatment ID: "); scanf("%d", &t.id);
    if(idExistsTreatment(t.id)){ printf("ID already exists.\n"); return; }
    printf("Name: "); getchar(); fgets(t.name, STR_MED, stdin); t.name[strcspn(t.name,"\n")] = 0;
    printf("Duration (min): "); scanf("%d", &t.durationMin);
    printf("Cost: "); scanf("%f", &t.cost);
    t.active=1;
    treatments[treatmentCount++] = t;
    printf("Treatment added.\n");
}

void adminUpdateTreatment(){
    int id; printf("Treatment ID to update: "); scanf("%d", &id);
    int idx = findTreatmentIndexById(id); if(idx<0){ printf("Not found.\n"); return; }
    Treatment *t = &treatments[idx]; getchar();
    char buf[STR_MED];
    printf("Name (%s): ", t->name); fgets(buf, STR_MED, stdin); if(buf[0]!='\n'){ buf[strcspn(buf,"\n")] = 0; strncpy(t->name, buf, STR_MED); }
    printf("Duration (%d): ", t->durationMin); fgets(buf, STR_MED, stdin); if(buf[0]!='\n'){ t->durationMin = atoi(buf);}    
    printf("Cost (%.2f): ", t->cost); fgets(buf, STR_MED, stdin); if(buf[0]!='\n'){ t->cost = (float)atof(buf);} 
    printf("Updated.\n");
}

void adminDeleteTreatment(){
    int id; printf("Treatment ID to delete: "); scanf("%d", &id);
    int idx = findTreatmentIndexById(id); if(idx<0){ printf("Not found.\n"); return; }
    treatments[idx].active = 0; 
    printf("Treatment deleted (soft).\n");
}

// ----------------------------- Authentication (Users)

int adminLogin(){
    char u[STR_SMALL], p[STR_SMALL];
    printf("Admin username: "); scanf("%s", u);
    printf("Admin password: "); scanf("%s", p);
    if(strcmp(u,ADMIN_USER)==0 && strcmp(p,ADMIN_PASS)==0) return 1;
    printf("Invalid admin credentials.\n");
    return 0;
}

int registerPatient(){
    if(patientCount>=MAX_PATIENTS){ printf("Patient list full.\n"); return -1; }
    Patient pt; memset(&pt,0,sizeof(pt));
    printf("Choose Patient ID (number): "); scanf("%d", &pt.id);
    if(idExistsPatient(pt.id)){ printf("ID already exists.\n"); return -1; }
    printf("Name: "); getchar(); fgets(pt.name, STR_MED, stdin); pt.name[strcspn(pt.name,"\n")] = 0;
    printf("Age: "); scanf("%d", &pt.age);
    printf("Gender: "); scanf("%s", pt.gender);
    printf("Phone: "); scanf("%s", pt.phone);
    printf("Password: "); scanf("%s", pt.password);
    pt.active=1;
    patients[patientCount++] = pt;
    printf("Registered. Login with your ID and password.\n");
    return pt.id;
}

int patientLogin(){
    int id; char pw[STR_SMALL];
    printf("Patient ID: "); scanf("%d", &id);
    printf("Password : "); scanf("%s", pw);
    int idx = findPatientIndexById(id);
    if(idx<0){ printf("No such patient.\n"); return -1; }
    if(strcmp(patients[idx].password, pw)==0) return id;
    printf("Wrong password.\n");
    return -1;
}

void patientChangePassword(int pid){
    int idx = findPatientIndexById(pid); if(idx<0) return;
    char cur[STR_SMALL], nw[STR_SMALL];
    printf("Current password: "); scanf("%s", cur);
    if(strcmp(cur, patients[idx].password)!=0){ printf("Incorrect.\n"); return; }
    printf("New password: "); scanf("%s", nw);
    strncpy(patients[idx].password, nw, STR_SMALL);
    printf("Password updated.\n");
}

// ----------------------------- Appointments

void userBookAppointment(int patientId){
    listDoctors(); listTreatments();
    int did, tid; char date[STR_SMALL], time[STR_SMALL];
    printf("Choose Doctor ID: "); scanf("%d", &did);
    printf("Choose Treatment ID: "); scanf("%d", &tid);
    int dIdx = findDoctorIndexById(did); if(dIdx<0){ printf("Doctor not found.\n"); return; }
    int tIdx = findTreatmentIndexById(tid); if(tIdx<0){ printf("Treatment not found.\n"); return; }
    if(!doctorTreats[dIdx][tIdx]){ printf("This doctor doesn't perform selected treatment.\n"); return; }
    printf("Date (DD-MM-YYYY): "); scanf("%s", date);
    printf("Time (HH:MM): "); scanf("%s", time);

    if(isSlotBusyDoctor(did,date,time)){ printf("Doctor already booked at that time.\n"); return; }
    if(isSlotBusyPatient(patientId,date,time)){ printf("You already have an appointment at that time.\n"); return; }

    if(apptCount>=MAX_APPTS){ printf("Appointment list full.\n"); return; }
    Appointment a; memset(&a,0,sizeof(a));
    a.id = NEXT_APPT_ID++;
    a.patientId = patientId; a.doctorId = did; a.treatmentId = tid;
    strncpy(a.date, date, STR_SMALL); strncpy(a.time, time, STR_SMALL);
    strcpy(a.status, "Pending"); a.active=1;
    appts[apptCount++] = a;
    printf("Appointment created with ID #%d (Pending approval).\n", a.id);
}

void userMyAppointments(int patientId){ listAppointments(patientId, -1); }

void userCancel(int patientId){
    int aid; printf("Appointment ID to cancel: "); scanf("%d", &aid);
    for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].id==aid && appts[i].patientId==patientId){
        if(strcmp(appts[i].status,"Approved")==0 || strcmp(appts[i].status,"Pending")==0){ strcpy(appts[i].status,"Cancelled"); printf("Cancelled.\n"); return; }
        else { printf("Cannot cancel (status: %s).\n", appts[i].status); return; }
    }
    printf("Not found.\n");
}

void adminApproveReject(){
    listAppointments(-1, -1);
    int aid; printf("Appointment ID: "); scanf("%d", &aid);
    for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].id==aid){
        if(strcmp(appts[i].status,"Pending")!=0){ printf("Only pending can be updated.\n"); return; }
        int c; printf("1=Approve 2=Reject: "); scanf("%d", &c);
        if(c==1) strcpy(appts[i].status, "Approved"); else if(c==2) strcpy(appts[i].status, "Rejected"); else { printf("Invalid.\n"); return; }
        printf("Updated.\n"); return;
    }
    printf("Not found.\n");
}

void adminReschedule(){
    int aid; printf("Appointment ID to reschedule: "); scanf("%d", &aid);
    for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].id==aid){
        int did = appts[i].doctorId; int pid = appts[i].patientId;
        char date[STR_SMALL], time[STR_SMALL];
        printf("New Date (DD-MM-YYYY): "); scanf("%s", date);
        printf("New Time (HH:MM): "); scanf("%s", time);
        if(isSlotBusyDoctor(did,date,time)){ printf("Doctor busy.\n"); return; }
        if(isSlotBusyPatient(pid,date,time)){ printf("Patient busy.\n"); return; }
        strncpy(appts[i].date,date,STR_SMALL); strncpy(appts[i].time,time,STR_SMALL);
        if(strcmp(appts[i].status,"Rejected")==0 || strcmp(appts[i].status,"Cancelled")==0) strcpy(appts[i].status,"Pending");
        printf("Rescheduled.\n"); return;
    }
    printf("Not found.\n");
}

void adminDeleteAppointment(){
    int aid; printf("Appointment ID to delete: "); scanf("%d", &aid);
    for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].id==aid){ appts[i].active=0; printf("Deleted (soft).\n"); return; }
    printf("Not found.\n");
}

// ----------------------------- Reports

void reportDailySummary(){
    char date[STR_SMALL]; printf("Date (DD-MM-YYYY): "); scanf("%s", date);
    int total=0, pending=0, approved=0, rejected=0, cancelled=0;
    for(int i=0;i<apptCount;i++) if(appts[i].active && dateEq(appts[i].date,date)){
        total++; if(strcmp(appts[i].status,"Pending")==0) pending++; else if(strcmp(appts[i].status,"Approved")==0) approved++; else if(strcmp(appts[i].status,"Rejected")==0) rejected++; else if(strcmp(appts[i].status,"Cancelled")==0) cancelled++; }
    printf("Total: %d | Approved: %d | Pending: %d | Rejected: %d | Cancelled: %d\n", total, approved, pending, rejected, cancelled);
}

void reportDoctorWise(){
    int did; printf("Doctor ID: "); scanf("%d", &did);
    int idx = findDoctorIndexById(did); if(idx<0){ printf("Not found.\n"); return; }
    int count=0; for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].doctorId==did && strcmp(appts[i].status,"Approved")==0) count++;
    printf("Doctor %s (ID %d) approved appointments: %d\n", doctors[idx].name, did, count);
}

void reportTreatmentDemand(){
    printLine(64);
    printf("%-6s %-22s %-14s\n","ID","Treatment","Approved Cnt");
    printLine(64);
    for(int t=0;t<treatmentCount;t++) if(treatments[t].active){
        int c=0; for(int i=0;i<apptCount;i++) if(appts[i].active && appts[i].treatmentId==treatments[t].id && strcmp(appts[i].status,"Approved")==0) c++;
        printf("%-6d %-22s %-14d\n", treatments[t].id, treatments[t].name, c);
    }
    printLine(64);
}

// ----------------------------- Menus

void adminMenu(){
    int ch=0; do{
        printf("\n===== ADMIN MENU =====\n");
        printf("1. List Doctors\n2. Add Doctor\n3. Update Doctor\n4. Delete Doctor\n5. Search Doctor by Specialization\n");
        printf("6. List Treatments\n7. Add Treatment\n8. Update Treatment\n9. Delete Treatment\n10. Map Treatment -> Doctor\n11. Unmap Treatment -> Doctor\n");
        printf("12. List Patients\n13. List Appointments\n14. Approve/Reject Appointment\n15. Reschedule Appointment\n16. Delete Appointment\n");
        printf("17. Report: Daily Summary\n18. Report: Doctor-wise Count\n19. Report: Treatment Demand\n20. Logout\nChoice: ");
        scanf("%d", &ch);
        switch(ch){
            case 1: listDoctors(); break;
            case 2: adminAddDoctor(); break;
            case 3: adminUpdateDoctor(); break;
            case 4: adminDeleteDoctor(); break;
            case 5: searchDoctorBySpecialization(); break;
            case 6: listTreatments(); break;
            case 7: adminAddTreatment(); break;
            case 8: adminUpdateTreatment(); break;
            case 9: adminDeleteTreatment(); break;
            case 10: adminMapTreatmentToDoctor(); break;
            case 11: adminUnmapTreatmentFromDoctor(); break;
            case 12: listPatients(); break;
            case 13: listAppointments(-1,-1); break;
            case 14: adminApproveReject(); break;
            case 15: adminReschedule(); break;
            case 16: adminDeleteAppointment(); break;
            case 17: reportDailySummary(); break;
            case 18: reportDoctorWise(); break;
            case 19: reportTreatmentDemand(); break;
            case 20: default: break;
        }
    }while(ch!=20);
}

void patientMenu(int patientId){
    int ch=0; do{
        printf("\n===== PATIENT MENU =====\n");
        printf("1. View Doctors\n2. View Treatments\n3. Book Appointment\n4. My Appointments\n5. Cancel Appointment\n6. Change Password\n7. Logout\nChoice: ");
        scanf("%d", &ch);
        switch(ch){
            case 1: listDoctors(); break;
            case 2: listTreatments(); break;
            case 3: userBookAppointment(patientId); break;
            case 4: userMyAppointments(patientId); break;
            case 5: userCancel(patientId); break;
            case 6: patientChangePassword(patientId); break;
            case 7: default: break;
        }
    }while(ch!=7);
}

int main(){
    seedDemo();
    int choice;
    while(1){
        printf("\n===== CLINIC APPOINTMENT SYSTEM =====\n");
        printf("1. Admin Login\n2. Patient Login\n3. Patient Register\n4. Exit\nChoice: ");
        if(scanf("%d", &choice)!=1){ break; }
        if(choice==1){ if(adminLogin()) adminMenu(); }
        else if(choice==2){ int pid = patientLogin(); if(pid>0) patientMenu(pid); }
        else if(choice==3){ registerPatient(); }
        else if(choice==4){ printf("Goodbye!\n"); break; }
        else { printf("Invalid choice.\n"); }
    }
    return 0;
}
