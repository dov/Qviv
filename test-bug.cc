  // Illustration of bug for 3x3 Format_Indexed8 image scaling
  #include <QImage>
  
  int main()
  {
    // Create a checkerpatterned 3x3 image
    QImage image(3,3,QImage::Format_Indexed8);
    image.setColorCount(2);
    image.setColor(0,0xff000000);
    image.setColor(1,0xffffffff);
  
    uchar *bits = image.bits();
    int bytesPerLine = image.bytesPerLine();
    for (int i=0; i<9; i++)
      bits[(i/3)*bytesPerLine+(i%3)]=i%2;
  
    // Save it
    image.save("bw-in.png");
    
    // If this line is commented out, everything is ok.
    image = image.convertToFormat(QImage::Format_RGB32);
  
    // Rescale image
    QImage outImage = image.scaled(image.height()*217, image.width()*217,
                                   Qt::KeepAspectRatio,Qt::FastTransformation);
    outImage.save("bw-out.png");
  }
