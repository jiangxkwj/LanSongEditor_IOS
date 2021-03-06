//
//  Pen.h
//  LanSongEditorFramework
//
//  Created by sno on 16/12/21.
//  Copyright © 2016年 sno. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "LanSongContext.h"
#import "LanSongOutput.h"
#import "LanSongFilter.h"
#import "LanSongTwoInputFilter.h"



typedef NS_ENUM(NSUInteger, PenTpye) {
    kVideoPen,
    kBitmapPen,
    kViewPen,
    kCameraPen,
    kMVPen,
    kCALayerPen,
    kDataPen
};


/**
 提示1:   ios版本的LanSong功能很强大, 为了滤镜部分和它兼容, 我们的图层继承自LanSong中的LanSongOutput,
         您可以直接使用LanSong的滤镜效果,并支持LanSong的各种扩展效果.
         与LanSongOutput的区别是: LanSongOutput只能做滤镜功能, 而我们是整个视频编辑SDK.
 
 提示2:  因为图层的的单词是Layer, 而'Layer'单词被IOS的UI使用了, 为了不使您代码中的对象命名混乱,
        我们用Pen这个单词作为图层的父类, 只是单词变化了,和Android版本的一样是图层的意思, 一样每个图层均支持移动缩放旋转滤镜等特性
 
 */
@interface Pen : LanSongOutput
{
      NSObject *framebufferLock;  //数据的同步锁. 内部使用.
}


/**
 *  当前图层的类型
 */
@property(readwrite, nonatomic) PenTpye penType;

/*
 图层过多的话, 则可以给每个图层设置一个TAG;
 */
@property(readwrite, nonatomic) NSString *tag;

/**
 内部使用.
 当然图层是否在运行.
 */
@property(nonatomic, assign,readonly)BOOL isRunning;

/**
 内部使用.
 */
@property BOOL isCamFrontMirror;
/**
 *  在绘制到容器上时的初始尺寸.   为固定值,不随图层的缩放变化而变化.
    如果你要获取当前画面的实时尺寸,则可以通过上面的frameBufferSize这个属性来获取. 缩放也是基于frameBufferSize进行的.
    
    此尺寸可以作为移动的参考.
 
 
  当前绘制原理是:ViewPen是等比例缩放到容器上, BitmapPen和ViewPen和CALayerPen, 则是1:1渲染到容器上.
 
   举例:视频是1280x720的视频, 容器尺寸是480x480,则增加到容器上后, 会自动缩放视频的尺寸,
      如果是横屏, 则视频的宽度被缩放成480, 高度被缩放成 480 x(视频的宽高比720/1280)=270; 则这里penSize的宽高是480x270,从而保证视频的宽高比一直.
      如果是竖屏, 则视频的高度被缩放到480, 宽度被缩放成 270, ....
 
 注意:后期会增加一些类似ImageView中的contentMode;其他各种缩放模式.
 */
@property CGSize penSize;


/**
 *  定义的容器尺寸
 */
@property(readwrite, nonatomic) CGSize drawPadSize;


/**
 当前图层在容器里的位置. 最里面是0, 最外面是图层最数量-1;
 */
@property int  indexInDrawPad;

/**
 *  当前时间戳
 可通过这样换算成秒:CGFloat frameTimeDifference = CMTimeGetSeconds(currentPTS)
 */
//@property CMTime currentPTS;
/**
 *  内部使用
 */
//@property LanSongRotationMode inputRotation;


/**
 当前图层是否隐藏, 
 可以用这个在新创建的图层做隐藏/显示的效果, 类似闪烁, 或创建好,暂时不显示等效果.
 */
@property(getter=isHidden) BOOL hidden;
/**
 角度值0--360度. 默认为0.0
 以视频的原视频角度为旋转对象,
 基本等同于CGAffineTransformRotate
 */
@property(readwrite, nonatomic)  CGFloat rotateDegree;
/**
 *  
 设置或读取当前画面的中心点的坐标像素值, 左上角为0,0.
 默认是中心点, 即:positionX=drawPadSize.width/2;
               positionY=drawPadSize.height/2;
 
 注意:这里的XY是画面中心点的坐标, 不是画面左上角!.
 
 */
@property(readwrite, nonatomic)  CGFloat positionX, positionY;

/**
 *  
 缩放因子, 大于1.0为放大, 小于1.0为缩小. 默认是1.0f
 如果是图片, 则默认以图片的宽高为缩放基数.来放大或缩小
 
 如果是视频,则以当前drawpad的大小为缩放基数, 来放大或缩小.
 
 注意: 此缩放, 是针对正要渲染的Pen进行缩放, 不会更改frameBufferSize和penSize.
 */
@property(readwrite, nonatomic)  CGFloat scaleWidth,scaleHeight;

/**
 直接缩放到的值,
 如果要等于把图片覆盖整个容器, 则值直接等于drawpadSize即可.
 */
@property(readwrite, nonatomic)  CGFloat scaleWidthValue,scaleHeightValue;


/**
 *  内部使用
 */
- (id)initWithDrawPadSize:(CGSize)size drawpadTarget:(id<LanSongInput>)target penType:(PenTpye) type;


/**
 内部使用.
 */
-(void)releasePen;
/**
 *  内部使用
 */
-(BOOL)decodeOneFrame;

/**
 *  内部使用
 */
-(void)loadParam:(LanSongContext*)context;
/**
 *  内部使用
 */
-(void)loadShader;
/**
 *  内部使用
 */
- (void)draw;
/**
 *  内部使用
 */
- (void)drawDisplay;

/**
 *  切换滤镜, 默认是没有滤镜. 
    因为IOS端的LanSong开源库很强大, 这里完全兼容LanSong的库,您也可以根据自己的情况扩展LanSong相关的效果.
 *
 *  @param filter 滤镜对象.
 */
-(void)switchFilter:(LanSongOutput<LanSongInput> *)filter;


/**
  切换滤镜, 切换到的滤镜, 可以有第二个输入源.

 @param filter 切换到的滤镜
 @param secondInput filter的第二个输入源, 一般用在各种Blend类型的滤镜中
 */
-(void)switchFilter:(LanSongTwoInputFilter *)filter secondInput:(LanSongOutput *)secondFilter;


/**
  切换滤镜, 这里是滤镜级联(滤镜叠加)

 举例1:
  3个滤镜级联:视频图层--经过 A滤镜 --->B滤镜--->C滤镜--->DrawPad编码
 则这里应该填写的是 startFilter=A滤镜;
  endFilter=C滤镜;
 B滤镜的使用代码是: [A滤镜 addTarget B滤镜];  [B滤镜 addTarget C滤镜];  C滤镜才是填入到我们endFilter中的值.
 
 举例2: 
    2个滤镜级联:视频图层-->A滤镜-->B滤镜-->DrawPad编码
 则这里 startFilter=A滤镜;
    endFilter=B滤镜;
 您代码中应该有 [A addTarget B]; 这样的代码.
 比如代码如下:
    LanSongSepiaFilter *sepiaFilter=[[LanSongSepiaFilter alloc] init];
    LanSongSwirlFilter *swirlFilter=[[LanSongSwirlFilter alloc] init];
    [sepiaFilter addTarget:swirlFilter];
 
    [camDrawPad.cameraPen switchFilterWithStartFilter:sepiaFilter endFilter:swirlFilter];
 
 @param startFilter 切换的第一个滤镜
 @param endFilter  切换的最后一个滤镜.
 */
-(void)switchFilterWithStartFilter:(LanSongOutput<LanSongInput> *)startFilter endFilter:(LanSongOutput<LanSongInput> *)endFilter;


/**
 内部使用
 */
- (void)startProcessing:(BOOL)isAutoMode;

-(void)endProcessing;

-(BOOL) isFrameAvailable;
-(void)setDriveDraw:(BOOL)is;
-(void)resetCurrentFrame;


@end
