# YUV GL Widget

Simple QOpenGLWidget subclass to display YUV420P frames using shaders. Could be used for other YUV formats.

Probably not optimal, but working ;)


### Usage example with AVFrame (ffmpeg)

Set OpenGL profile in main
```
// Set core profile
QSurfaceFormat defaultFormat = QSurfaceFormat::defaultFormat();
defaultFormat.setProfile(QSurfaceFormat::CoreProfile);
defaultFormat.setVersion(3, 3); // Adapt to your system
QSurfaceFormat::setDefaultFormat(defaultFormat);
```

Setup widget and update it
```
YUVGLWidget yuvwidget(defaultFormat);
//yuvwidget.setFrameSize(3840, 2160); // Default is 4K frame

AVFrame* frame = ...;
yuvwidget.setYPixels(frame->data[0], frame->linesize[0]);
yuvwidget.setUPixels(frame->data[1], frame->linesize[1]);
yuvwidget.setVPixels(frame->data[2], frame->linesize[2]);
yuvwidget.update();
```

### Credits
Based off http://roxlu.com/2014/039/decoding-h264-and-yuv420p-playback
