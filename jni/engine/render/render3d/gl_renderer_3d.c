//#include <jni.h>


#include "gl_renderer_3d.h"

//#define  LOG_TAG    "libgl2jni"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
	GLint error;
    for (error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static void init() {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    glShadeModel(GL_SMOOTH);						// ������Ӱƽ��
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			// ��ɫ����
    glClearDepthf(1.0f);							// ������Ȼ���
    glEnable(GL_DEPTH_TEST);						// ������Ȳ���
    glDepthFunc(GL_LEQUAL);							// ������Ȳ��Ե�����
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ����ϵͳ��͸�ӽ�������

}

static const GLfloat gVertices[] = {
		0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, -1.0f,

		0.0f, 1.0f, 0.0f,
		1.0f,-1.0f, -1.0f,
		-1.0f,-1.0f, -1.0f,

		0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f
};

static const GLfloat gColors[] = {
		1.0f,0.0f,0.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,

		1.0f,0.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f,

		1.0f,0.0f,0.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,

		1.0f,0.0f,0.0f, 1.0f,
		0.0f,0.0f,1.0f, 1.0f,
		0.0f,1.0f,0.0f, 1.0f
};

static GLfloat rtri;  //�����ε���ת����
static GLfloat rquad;  //�ı��ε���ת����

static const GLfloat PI = 3.1415f;

static void _gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLfloat top = zNear * ((GLfloat) tan(fovy * PI / 360.0));
	GLfloat bottom = -top;
	GLfloat left = bottom * aspect;
	GLfloat right = top * aspect;
	glFrustumf(left, right, bottom, top, zNear, zFar);
}

PUBLIC void renderer3d_init(int width, int height)
{
	init();
    if (height==0)								// ��ֹ�����
      {
              height=1;							// ��Height��Ϊ1
      }

      glViewport(0, 0, width, height);					// ���õ�ǰ���ӿ�
      glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
      glLoadIdentity();							// ����ͶӰ����

      GLfloat ratio = (GLfloat)width/(GLfloat)height;
      // �����ӿڵĴ�С
      _gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  //    glOrthof(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);

      glMatrixMode(GL_MODELVIEW);						// ѡ��ģ�͹۲����
      glLoadIdentity();							// ����ģ�͹۲����
}


void renderer3d_draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
    glLoadIdentity();					// ����ģ�͹۲����
    glTranslatef(0.0f,0.0f, -6.0f);				// ������Ļ 6.0

    glRotatef(rtri,0.0f,1.0f,0.0f);				// ��Y����ת������

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, gColors);

    glVertexPointer(3, GL_FLOAT, 0, gVertices);
    glDrawArrays(GL_TRIANGLES, 0, 12);

	rtri += 0.2f;						// ���������ε���ת����
	//LOGI("xxxxx");

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glFlush();
}

//    JNIEXPORT void JNICALL Java_com_example_hellojni_OpenGlJniLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
//    JNIEXPORT void JNICALL Java_com_example_hellojni_OpenGlJniLib_step(JNIEnv * env, jobject obj);
//    JNIEXPORT void JNICALL Java_com_example_hellojni_OpenGlJniLib_create(JNIEnv * env, jobject obj);
////extern "C" {
////};
//
//JNIEXPORT void JNICALL Java_com_example_hellojni_OpenGlJniLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
//{
//	LOGI("jni: gl has inited!!");
//    resize(width, height);
////    init2d();
//}
//
//static double stime = 0;
//static int count = 0;
//
///* Return current time in milliseconds */
//double time_util_now_ms(void) {
//    struct timeval tv;
//    gettimeofday(&tv, NULL);
//    return (double)(tv.tv_sec*1000.0 + tv.tv_usec/1000.0);
//}
//
//JNIEXPORT void JNICALL Java_com_example_hellojni_OpenGlJniLib_step(JNIEnv * env, jobject obj)
//{
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
////    glFlush();
//    renderFrame();
////    draw_figure();
//	 count++;
//	 if (time_util_now_ms() - stime >= 1000) {
//		 LOGE("xxxxxxxxxxx11  %d", (count));
//		 stime = time_util_now_ms();
//		 count = 0;
//	 }
//}
//
//JNIEXPORT void JNICALL Java_com_example_hellojni_OpenGlJniLib_create(JNIEnv * env, jobject obj)
//{
//	LOGI("jni: gl has created!!");
////	init();
//}
