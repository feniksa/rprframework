#include "GLWidget.h"
#include "MainWindow.h"

#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <qmath.h>

GLWidget::GLWidget(MainWindow* parent) : /*: QOpenGLWidget(parent),*/
m_mainWindow(parent)
{
	setMinimumSize(300, 250);
}

GLWidget::~GLWidget()
{
	reset();
}

void GLWidget::reset()
{    // And now release all OpenGL resources.
	makeCurrent();
	
	delete m_texture;
	m_texture = nullptr;
	delete m_program1;
	m_program1 = nullptr;
	delete m_vshader1;
	m_vshader1 = nullptr;
	delete m_fshader1;
	m_fshader1 = nullptr;
	m_vbo1.destroy();
	
	doneCurrent();
	
	// We are done with the current QOpenGLContext, forget it. If there is a
	// subsequent initialize(), that will then connect to the new context.
	QObject::disconnect(m_contextWatchConnection);
}

void GLWidget::setScaling(int scale)
{
	if (scale > 30)
		m_fScale = 1 + qreal(scale - 30) / 30 * 0.25;
	else if (scale < 30)
		m_fScale =  1 - (qreal(30 - scale) / 30 * 0.25);
	else
		m_fScale = 1;
}

void GLWidget::setTransparent(bool transparent)
{
	setAttribute(Qt::WA_AlwaysStackOnTop, transparent);
	m_transparent = transparent;
// Call update() on the top-level window after toggling AlwayStackOnTop to make sure
// the entire backingstore is updated accordingly.
	window()->update();
}

void GLWidget::resizeGL(int, int)
{
	/*if (m_mainWindow) {
	if (!m_btn) {
	    m_btn = new QPushButton("\nAdd widget\n", this);
	    connect(m_btn, &QPushButton::clicked, this, [this] { m_mainWindow->addNew(); });
	}
	m_btn->move(20, 80);
	if (!m_btn2) {
	    m_btn2 = new QPushButton("\nI prefer tabbed widgets\n", this);
	    connect(m_btn2, &QPushButton::clicked, this, [this] { m_mainWindow->showNewWindow(); });
	}
	m_btn2->move(20, 160);
	}*/
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	m_vshader1 = new QOpenGLShader(QOpenGLShader::Vertex);
	const char *vsrc1 =
	"attribute highp vec4 vertex;\n"
	"attribute mediump vec3 normal;\n"
	"uniform mediump mat4 matrix;\n"
	"varying mediump vec4 color;\n"
	"void main(void)\n"
	"{\n"
	"    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
	"    float angle = max(dot(normal, toLight), 0.0);\n"
	"    vec3 col = vec3(0.40, 1.0, 0.0);\n"
	"    color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);\n"
	"    color = clamp(color, 0.0, 1.0);\n"
	"    gl_Position = matrix * vertex;\n"
	"}\n";
	m_vshader1->compileSourceCode(vsrc1);

	m_fshader1 = new QOpenGLShader(QOpenGLShader::Fragment);
	const char *fsrc1 =
	"varying mediump vec4 color;\n"
	"void main(void)\n"
	"{\n"
	"    gl_FragColor = color;\n"
	"}\n";
	m_fshader1->compileSourceCode(fsrc1);

	m_program1 = new QOpenGLShaderProgram;
	m_program1->addShader(m_vshader1);
	m_program1->addShader(m_fshader1);
	m_program1->link();
	
	m_vertexAttr1 = m_program1->attributeLocation("vertex");
	m_normalAttr1 = m_program1->attributeLocation("normal");
	m_matrixUniform1 = m_program1->uniformLocation("matrix");
	
	m_fAngle = 0;
	m_fScale = 1;
	
	// Use a vertex buffer object. Client-side pointers are old-school and should be avoided.
	m_vbo1.create();
	m_vbo1.bind();
	
	//m_vbo1.allocate(buf.constData(), buf.count() * sizeof(GLfloat));
	m_vbo1.release();
	
	/*m_texture = new QOpenGLTexture(QImage(":/qt.png"));
	
	m_vshader1 = new QOpenGLShader(QOpenGLShader::Vertex);
	const char *vsrc1 =
	"attribute highp vec4 vertex;\n"
	"attribute mediump vec3 normal;\n"
	"uniform mediump mat4 matrix;\n"
	"varying mediump vec4 color;\n"
	"void main(void)\n"
	"{\n"
	"    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
	"    float angle = max(dot(normal, toLight), 0.0);\n"
	"    vec3 col = vec3(0.40, 1.0, 0.0);\n"
	"    color = vec4(col * 0.2 + col * 0.8 * angle, 1.0);\n"
	"    color = clamp(color, 0.0, 1.0);\n"
	"    gl_Position = matrix * vertex;\n"
	"}\n";
	m_vshader1->compileSourceCode(vsrc1);

	m_fshader1 = new QOpenGLShader(QOpenGLShader::Fragment);
	const char *fsrc1 =
	"varying mediump vec4 color;\n"
	"void main(void)\n"
	"{\n"
	"    gl_FragColor = color;\n"
	"}\n";
	m_fshader1->compileSourceCode(fsrc1);

	m_program1 = new QOpenGLShaderProgram;
	m_program1->addShader(m_vshader1);
	m_program1->addShader(m_fshader1);
	m_program1->link();

	m_vertexAttr1 = m_program1->attributeLocation("vertex");
	m_normalAttr1 = m_program1->attributeLocation("normal");
	m_matrixUniform1 = m_program1->uniformLocation("matrix");

	m_vshader2 = new QOpenGLShader(QOpenGLShader::Vertex);
	const char *vsrc2 =
	"attribute highp vec4 vertex;\n"
	"attribute highp vec4 texCoord;\n"
	"attribute mediump vec3 normal;\n"
	"uniform mediump mat4 matrix;\n"
	"varying highp vec4 texc;\n"
	"varying mediump float angle;\n"
	"void main(void)\n"
	"{\n"
	"    vec3 toLight = normalize(vec3(0.0, 0.3, 1.0));\n"
	"    angle = max(dot(normal, toLight), 0.0);\n"
	"    gl_Position = matrix * vertex;\n"
	"    texc = texCoord;\n"
	"}\n";
	m_vshader2->compileSourceCode(vsrc2);

	m_fshader2 = new QOpenGLShader(QOpenGLShader::Fragment);
	const char *fsrc2 =
	"varying highp vec4 texc;\n"
	"uniform sampler2D tex;\n"
	"varying mediump float angle;\n"
	"void main(void)\n"
	"{\n"
	"    highp vec3 color = texture2D(tex, texc.st).rgb;\n"
	"    color = color * 0.2 + color * 0.8 * angle;\n"
	"    gl_FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);\n"
	"}\n";
	m_fshader2->compileSourceCode(fsrc2);

	m_program2 = new QOpenGLShaderProgram;
	m_program2->addShader(m_vshader2);
	m_program2->addShader(m_fshader2);
	m_program2->link();
	
	m_vertexAttr2 = m_program2->attributeLocation("vertex");
	m_normalAttr2 = m_program2->attributeLocation("normal");
	m_texCoordAttr2 = m_program2->attributeLocation("texCoord");
	m_matrixUniform2 = m_program2->uniformLocation("matrix");
	m_textureUniform2 = m_program2->uniformLocation("tex");
	
	m_fAngle = 0;
	m_fScale = 1;
	
	createGeometry();
	
	// Use a vertex buffer object. Client-side pointers are old-school and should be avoided.
	m_vbo1.create();
	m_vbo1.bind();
	// For the cube all the data belonging to the texture coordinates and
	// normals is placed separately, after the vertices. Here, for the Qt logo,
	// let's do something different and potentially more efficient: create a
	// properly interleaved data set.
	const int vertexCount = m_vertices.count();
	QList<GLfloat> buf;
	buf.resize(vertexCount * 3 * 2);
	GLfloat *p = buf.data();
	for (int i = 0; i < vertexCount; ++i) {
	*p++ = m_vertices[i].x();
	*p++ = m_vertices[i].y();
	*p++ = m_vertices[i].z();
	*p++ = m_normals[i].x();
	*p++ = m_normals[i].y();
	*p++ = m_normals[i].z();
	}
	m_vbo1.allocate(buf.constData(), buf.count() * sizeof(GLfloat));
	m_vbo1.release();
	
	createBubbles(bubbleNum - m_bubbles.count());*/
	
	// A well-behaved QOpenGLWidget releases OpenGL resources not only upon
	// destruction, but also when the associated OpenGL context disappears. If
	// the widget continues to exist, the context's destruction will be
	// followed by a call to initialize(). This is not strictly mandatory in
	// widgets that never change their parents.
	m_contextWatchConnection = QObject::connect(context(), &QOpenGLContext::aboutToBeDestroyed, context(), [this] { reset(); });
}

void GLWidget::paintGL()
{
	QPainter painter;
	painter.begin(this);
	
	painter.beginNativePainting();
	
	glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), m_transparent ? 0.0f : 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	
	QMatrix4x4 modelview;
	modelview.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
	modelview.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
	modelview.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
	modelview.scale(m_fScale);
	modelview.translate(0.0f, -0.2f, 0.0f);
	
	m_program1->bind();
	m_program1->setUniformValue(m_matrixUniform1, modelview);
	m_program1->release();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	painter.endNativePainting();
	
	if (const int elapsed = m_time.elapsed()) {
		QString framesPerSecond;
		framesPerSecond.setNum(m_frames /(elapsed / 1000.0), 'f', 2);
		painter.setPen(m_transparent ? Qt::black : Qt::white);
		painter.drawText(20, 40, framesPerSecond + " paintGL calls / s");
	}

	painter.end();

	if (!(m_frames % 100)) {
		m_time.start();
		m_frames = 0;
	}

	++m_frames;

	// When requested, follow the ideal way to animate: Rely on
	// blocking swap and just schedule updates continuously.
	if (!m_mainWindow || !m_mainWindow->timerEnabled())
		update();
}

