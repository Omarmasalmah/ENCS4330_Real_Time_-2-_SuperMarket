
// start opengl code.
#include "local.h"
#include <GL/glut.h>

// Functions for openGL
void display(void);
void reshape(int w, int h);
void timer(int z);
void drawTellers();
void drawText();
void renderText(int, int, void *, char *);
void readFromSharedMemory();
void drawDataHall();
void drawMainHall();
struct ShelvesSharedmemory *shelves;
struct StorageSharedmemory *storage;
// Parameters will cahange by reading from shared memory
//char buffer[256];
int NumberOfProductsInStorage=0;
int NumberOfProductsInSuperMarket=0;
int Time=0;
int shelves_quantity_on_shelves[MaxItems]; 
int shelves_id[MaxItems];
int shelves_quantity_to_refill[MaxItems];  

int storage_quantity[MaxItems];    
int storage_id[MaxItems];
void alarmHandler(int signum) {
    Time++;
    // Add your code to handle the alarm here
}                      

int rate = 1000 / 120;

int main(int argc, char **argv)
{
    key_t key_shelves = ftok(".", KEY_SHELVES);
    if (key_shelves == -1) {
        perror("ftok shelves");
        exit(EXIT_FAILURE);
    }
    // Get the shared memory segment ID for shelves
    int shmid_shelves = shmget(key_shelves, sizeof(struct ShelvesSharedmemory), 0666);

    if (shmid_shelves == -1) {
        perror("shmget shelves");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment for shelves
    shelves = shmat(shmid_shelves, NULL, 0);

    if (shelves == (struct ShelvesSharedmemory *)-1) {
        perror("shmat shelves");
        exit(EXIT_FAILURE);
    }

    key_t key_storage = ftok(".", KEY_STORAGE);

    if (key_storage == -1) {
        perror("ftok storage");
        exit(EXIT_FAILURE);
    }

    // Get the shared memory segment ID for storage
    int shmid_storage = shmget(key_storage, sizeof(struct StorageSharedmemory), 0666);

    if (shmid_storage == -1) {
        perror("shmget storage");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory segment for storage
    storage = shmat(shmid_storage, NULL, 0);

    if (storage == (struct StorageSharedmemory *)-1) {
        perror("shmat storage");
        exit(EXIT_FAILURE);
    }
    signal(SIGALRM, alarmHandler);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Super Market");
    glLineWidth(5);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    return 0;
}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 0, 255);

    // Drawing objects in the scene

    glLoadIdentity();
    drawDataHall();
    drawTellers();
    drawMainHall();
    readFromSharedMemory();
    drawText();
    glutSwapBuffers();
}

void reshape(int w, int h)
{

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -20, 20);
    glMatrixMode(GL_MODELVIEW);
}

// Timer function to control the animation and display FPS
void timer(int z)
{

    glutTimerFunc(rate, timer, 0);
    glutPostRedisplay();

}

// Function for drawing supermarket background image

void drawTellers()

{
glBegin(GL_POLYGON_BIT);

    glColor3f(0.1, 0.9, 0.4);

    glVertex2i(20, 12);

    glVertex2i(14, 12);

    glVertex2i(20, 6);

    glVertex2i(14, 6);

    glEnd();


    glBegin(GL_POLYGON_BIT);

    glColor3f(0.6, 0.4, 0.9);

    glVertex2i(20, 6);

    glVertex2i(14, 6);

    glVertex2i(20, 0);

    glVertex2i(14, 0);

    glEnd();


    glBegin(GL_POLYGON_BIT);

    glColor3f(0.356, 0.8, 0.999);

    glVertex2i(20, 0);

    glVertex2i(14, 0);

    glVertex2i(20, -6);

    glVertex2i(14, -6);

    glEnd();


    glBegin(GL_POLYGON_BIT);

    glColor3f(0.7, 0.8, 0.26);

    glVertex2i(20, -6);

    glVertex2i(14, -6);

    glVertex2i(20, -15);

    glVertex2i(14, -15);

    glEnd();


}

void drawMainHall()

{

    glColor3f(0, 0, 255); // Blue color

    glBegin(GL_LINES);

    glVertex2f(-20.0, -17.0);

    glVertex2f(-11.0, -17.0);

    glEnd();


    glBegin(GL_LINES);

    glVertex2f(-20.0, 13.0);

    glVertex2f(-11.0, 13.0);

    glEnd();


    glBegin(GL_LINES);

    glVertex2f(-11.0, -17.0);

    glVertex2f(-11.0, 13.0);

    glEnd();

    glFlush();
}



void drawDataHall()

{

    glColor3ub(255, 0, 255);

    glBegin(GL_LINES);

    glVertex2f(-10.0, -17.0);

    glVertex2f(-10.0, 13.0);

    glEnd();


    glBegin(GL_LINES);

    glVertex2f(-10.0, 13.0);

    glVertex2f(500.0, 13.0);

    glEnd();


    glBegin(GL_LINES);

    glVertex2f(-10.0, -17.0);

    glVertex2f(50.0, -17.0);

    glEnd();

    glFlush();
}

// Function for drawing text
void drawText()

{

    glColor3f(255, 255, 255);

    char

        supermarketText[30] = "Supermarket",

        storageText[30] = "Storage ",
        
        timeText[30] = "Time:",
        
        Timing[30]= "",

        productText[30] = "Product ",

        quantityText[30] = "Quantity ",
        
        
        productNameInSupermarket[30] = "Product Name",

        refillSupermarket[30] = "Quantity to Refill",

        shelveSupermarket[30] = "Quantity on Shelve",
        
        messageSupermarket[30] = "Messages",
        

        ManagerMessages[20] = "Manager",
        
        ManagerMessages2[20] ="went to get more",
        
        ManagerMessages3[20] ="from storage",
        
        Fill[20] = "Fill by",
        
        notFill[20] = "Filled",



        CustomersNum[30] = "",

        shelvesProductName[50] = "",

        quantity_to_refill[5] = "",

        quantity_on_shelves[5] = "",
        
        
        storageProductName[50] = "",

        storageQuantity[7] = "";
        



      sprintf(Timing, "%d", Time);   
      sprintf(CustomersNum, "The number of customers: %d", countProcesses("customer"));     

      renderText(-18, 18, GLUT_BITMAP_TIMES_ROMAN_24, CustomersNum);
      renderText(10, 18, GLUT_BITMAP_TIMES_ROMAN_24, Timing);


		// Supermarket Window
		
    for (int i = 0; i < MaxItems; ++i)      

    {
    
        if (shelves_id[i]==0 && shelves->quantity_to_refill[i]==0){continue;}
        sprintf(shelvesProductName, "%s", shelves->Name[i]);                             //shelves_name

        renderText(-9.0 +  i * 4, 9, GLUT_BITMAP_TIMES_ROMAN_24, shelvesProductName);
        

        
        if (shelves->quantity_on_shelves[i]==0 && shelves->Filling[i]==0 && shelves_id[i]==0 ){
        	renderText(-9.0 + i * 4, -3, GLUT_BITMAP_TIMES_ROMAN_24, "Empty!");
        }
        else{
        sprintf(quantity_to_refill, "%d", shelves_quantity_to_refill[i]);             //Shelve to relif

        renderText(-9.0 + i * 4, 3, GLUT_BITMAP_TIMES_ROMAN_24, quantity_to_refill);
        
        sprintf(quantity_on_shelves, "%d", shelves->quantity_on_shelves[i]);          //on shelve

        renderText(-9.0 + i * 4, -3, GLUT_BITMAP_TIMES_ROMAN_24, quantity_on_shelves);
        if (shelves->Filling[i]==1){
            renderText(-9.0 + i * 4, -6, GLUT_BITMAP_HELVETICA_18, ManagerMessages);
            renderText(-9.0 + i * 4, -9, GLUT_BITMAP_HELVETICA_18, ManagerMessages2);
            renderText(-9.0 + i * 4, -12, GLUT_BITMAP_HELVETICA_18, ManagerMessages3);
        }
        else if (shelves->Filling[i]==2 ){
       
            renderText(-9.0 + i * 4, -14, GLUT_BITMAP_TIMES_ROMAN_24, Fill);
            renderText(-9.0 + i * 4, -16, GLUT_BITMAP_TIMES_ROMAN_24, "Employees");
        
        }
        else{
            renderText(-9.0 + i * 4, -14, GLUT_BITMAP_TIMES_ROMAN_24, notFill);
        
        }
        }
        
        
    }
    
    
    
    	// Storage Window
    	
    for (int i = 0; i < MaxItems; ++i)      

    {
    	if (storage_id[i]==0 && storage->quantity_on_shelves[i]==0){continue;}
    	
        sprintf(storageProductName, "%s", storage->Name[i]);                             //storage_name

        renderText(-19, 7 - i * 3, GLUT_BITMAP_TIMES_ROMAN_24, storageProductName);
        if (storage->quantity[i]==0){
        	renderText(-15, 7 - i * 3, GLUT_BITMAP_TIMES_ROMAN_24, "Empty!");
        }
        else{
        sprintf(storageQuantity, "%d", storage->quantity[i]);             //Quantity

        renderText(-14, 7 - i * 3, GLUT_BITMAP_TIMES_ROMAN_24, storageQuantity);
        }

     }

  

   

    renderText(-1, 15, GLUT_BITMAP_TIMES_ROMAN_24, supermarketText);

    renderText(-18, 15, GLUT_BITMAP_TIMES_ROMAN_24, storageText);
    renderText(7, 18, GLUT_BITMAP_TIMES_ROMAN_24, timeText);

    renderText(-19, 10, GLUT_BITMAP_TIMES_ROMAN_24, productText);

    renderText(-15, 10, GLUT_BITMAP_TIMES_ROMAN_24, quantityText);
    
    
     renderText(14, 9, GLUT_BITMAP_HELVETICA_18, productNameInSupermarket);

    renderText(14, 3, GLUT_BITMAP_HELVETICA_18, refillSupermarket);

    renderText(14, -3, GLUT_BITMAP_HELVETICA_18, shelveSupermarket);
    
    renderText(15, -11, GLUT_BITMAP_HELVETICA_18, messageSupermarket);
    


}

// Function for render text

void renderText(int x, int y, void *font, char *string)

{

    glRasterPos2f(x, y);

    int len, i;

    len = strlen(string);

    for (i = 0; i < len && string[i] != '\0'; i++)

    {

        glutBitmapCharacter(font, string[i]);
    }
}


// Function for read data from shared memory

void readFromSharedMemory()

{

	readSharedMemoryData(&NumberOfProductsInStorage, &NumberOfProductsInSuperMarket,
			  shelves_quantity_on_shelves, shelves_quantity_to_refill,
                          storage_quantity, shelves_id, storage_id);

}







