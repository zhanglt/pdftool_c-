#pragma execution_character_set("utf-8")//防止出现中文乱码
#include "include/pdf2image/pdf2ImageThreadSingle.h"
#include <QDebug>
#include <QThread>
#include <QElapsedTimer>
#include "function.h"

/*
pdf2imageThreadSingle::pdf2imageThreadSingle(QMutex *mutex,int *count) :m_mutex(mutex)
{
    this->count=count;
    setAutoDelete(true);
}
*/
pdf2imageThreadSingle::pdf2imageThreadSingle(QObject* parent) : QObject(parent), QRunnable()
//pdf2imageThreadSingle::pdf2imageThreadSingle() :  QRunnable()
{
    ImagethreadPool.setMaxThreadCount(1);
    setAutoDelete(true);

}

void pdf2imageThreadSingle::setSourceFile(QString sourceFile) {
    m_sourceFile = sourceFile;
}
void pdf2imageThreadSingle::setTargetFile(QString targetFile){

    m_targetFile=targetFile;
}
void pdf2imageThreadSingle::setImagePath(QString imagePath) {
    m_imagePath = imagePath;
}

void pdf2imageThreadSingle::setResolution(int resolution){
    m_resolution=resolution;

}
void pdf2imageThreadSingle::run()
{
  // qDebug()<<"最终文件路径"<<m_targetFile;
  // qDebug()<<"原来文件路径"<<m_sourceFile;
  // qDebug()<<"图像文件路径"<<m_imagePath;
    int num =pdf2image(m_sourceFile.toStdString(),m_imagePath.toStdString(),m_resolution);

   // for (int i=0;i<num;i++){
        //qDebug()<<m_imagePath+"/"+QString::number(i)+".png";
        images2pdf(m_imagePath.toStdString(),m_targetFile.toStdString(),num);
    //}

    emit addFinish();
}



int pdf2imageThreadSingle::pdf2image(string pdfFile,string imagePath,int resolution){
    /*
    if (imagePath==""){
        //imagePath=extractDirectory(pdfFilename);
        imagePath= imagePath.replace(imagePath.find("_out_"), 5, "_image_");
        //string  filename=extractFileName(pdfFilename);
        //if (createDirectory(QString::fromStdString(imagePath) +"/"+QString::fromStdString(filename))) {
        //    imagePath=imagePath+"/"+filename+"/";
        // }
        createDirectory(QString::fromStdString(imagePath));

    }*/
    // qDebug()<<"imagePath"<<QString::fromStdString(imagePath);
    // qDebug()<<"pdfFile"<<QString::fromStdString(pdfFile);
    //std::string sfile(pdfFile);
    fz_context* ctx =  fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
    fz_register_document_handlers(ctx);
    fz_document* doc =  fz_open_document(ctx, pdfFile.c_str());
    if (!doc) {
        qDebug()<<"文件处理错误："<<QString::fromStdString(pdfFile);
        return 1;
    }
    int num = fz_count_pages(ctx, doc);
    // qDebug()<< "page:" << num << endl;
    float zoom = (float)resolution / (float)72;
    fz_matrix ctm = fz_scale(zoom, zoom);

    for(int i=0; i<num; i++)
    {
        string fileName = imagePath+"/"+to_string(i)+ ".png";
        fz_pixmap* pix = fz_new_pixmap_from_page_number(ctx, doc, i, ctm, fz_device_rgb(ctx), 0);
        fz_save_pixmap_as_png(ctx, pix, fileName.c_str());
        fz_drop_pixmap(ctx, pix);
    }

    fz_drop_document(ctx, doc);
    fz_drop_context(ctx);
    return num;

}
