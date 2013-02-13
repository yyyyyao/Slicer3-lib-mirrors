#ifndef __vtkEMSegmentLogic_h
#define __vtkEMSegmentLogic_h

// Slicer includes
#include <vtkSlicerModuleLogic.h>
#include <vtkSlicerConfigure.h> // For Slicer3_USE_KWWIDGETS

// EMSegment includes
#include "vtkEMSegment.h"
#include "vtkSlicerCommonInterface.h"

#ifdef Slicer3_USE_KWWIDGETS
  class vtkMRMLAtlasCreatorNode;
#endif

class vtkEMSegmentMRMLManager;
class vtkGridTransform;
class vtkImageData;
class vtkImageEMLocalClass;
class vtkImageEMLocalGenericClass;
class vtkImageEMLocalSegmenter;
class vtkImageEMLocalSuperClass;
class vtkImageLevelSets;
class vtkMatrix4x4;
class vtkMRMLScalarVolumeNode;
class vtkMRMLVolumeNode;
class vtkSlicerApplicationLogic;
class vtkTransform;

class VTK_EMSEGMENT_EXPORT vtkEMSegmentLogic: public vtkSlicerModuleLogic
{
public:
  static vtkEMSegmentLogic *New();
  
  vtkTypeMacro(vtkEMSegmentLogic,vtkSlicerModuleLogic);

  // Description: The name of the Module---this is used to construct
  // the proc invocations
  vtkGetStringMacro(ModuleName)
  vtkSetStringMacro(ModuleName)

  vtkGetStringMacro(CurrentTmpFileName)
  vtkSetStringMacro(CurrentTmpFileName)
  
  char* mktemp_file(const char* postfix);
  char* mktemp_dir();

  //
  // actions
  //
  virtual bool StartPreprocessingInitializeInputData();
  //BTX
  std::string GetTclGeneralDirectory();
  //ETX

  // Used within StartSegmentation to copy data from the MRMLManager
  // to the segmenter algorithm.  Possibly useful for research
  // purposes.
  virtual void CopyDataToSegmenter(vtkImageEMLocalSegmenter* segmenter);

  //
  // progress bar related functions: not currently used, likely to
  // change
  vtkGetStringMacro(ProgressCurrentAction)
  vtkGetMacro(ProgressGlobalFractionCompleted, double)
  vtkGetMacro(ProgressCurrentFractionCompleted, double)

  //
  // MRML Related Methods.  The collection of MRML nodes for the
  // EMSegmenter is complicated.  Therefore, the management of these
  // nodes are delegated to the vtkEMSegmentMRMLManager class.
  vtkGetObjectMacro(MRMLManager, vtkEMSegmentMRMLManager)

  // Register all the nodes used by this module with the current MRML
  // scene.
  virtual void RegisterMRMLNodesWithScene();

  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void SetAndObserveMRMLScene(vtkMRMLScene* scene);

  // this is needed in Slicer4 to properly listen to the nodeAdded and nodeRemoved events
  void InitializeEventListeners();

  virtual void ProcessMRMLEvents(vtkObject *caller, unsigned long event, void *callData);

  // ProcessMRMLEvents is renamed into ProcessMRMLSceneEvents in Slicer4
  virtual void ProcessMRMLSceneEvents(vtkObject *caller,
                                      unsigned long event,
                                      void *callData);

  // events to observe
  virtual vtkIntArray* NewObservableEvents();

  void
      StartPreprocessingResampleAndCastToTarget(vtkMRMLVolumeNode* movingVolumeNode, vtkMRMLVolumeNode* fixedVolumeNode, vtkMRMLVolumeNode* outputVolumeNode);

  static void
      TransferIJKToRAS(vtkMRMLVolumeNode* volumeNode, int ijk[3], double ras[3]);
  static void
      TransferRASToIJK(vtkMRMLVolumeNode* volumeNode, double ras[3], int ijk[3]);

  static double GuessRegistrationBackgroundLevel(vtkMRMLVolumeNode* volumeNode);

  static void
      SlicerImageResliceWithGrid(vtkMRMLVolumeNode* inputVolumeNode, vtkMRMLVolumeNode* outputVolumeNode, vtkMRMLVolumeNode* outputVolumeGeometryNode, vtkGridTransform* outputRASToInputRASTransform, int iterpolationType, double backgroundLevel);

  // utility---should probably go to general slicer lib at some point
  static void
      SlicerImageReslice(vtkMRMLVolumeNode* inputVolumeNode, vtkMRMLVolumeNode* outputVolumeNode, vtkMRMLVolumeNode* outputVolumeGeometryNode, vtkTransform* outputRASToInputRASTransform, int iterpolationType, double backgroundLevel);

  // Helper Classes for tcl 
  void PrintTextNoNewLine(char *TEXT);
  void PrintText(char *TEXT);

  void DefineValidSegmentationBoundary();
  void AutoCorrectSpatialPriorWeight(vtkIdType nodeID);

  // This function is part of the EMSegmenter's stable API
  vtkMRMLScalarVolumeNode
      * AddArchetypeScalarVolume(const char* filename, const char* volname, vtkSlicerApplicationLogic* appLogic, vtkMRMLScene* mrmlScene);

  vtkMRMLScalarVolumeNode
      * AddArchetypeScalarVolume(const char* filename, const char* volname, vtkSlicerApplicationLogic* appLogic, vtkMRMLScene* mrmlScene, bool centered);

  //BTX
  std::string GetErrorMessage()
  {
    return this->ErrorMsg;
  }
  //ETX 

  virtual void CreateOutputVolumeNode();

  void SubParcelateSegmentation(vtkImageData* origSegmentation,  vtkImageData* currentParcellation,  vtkIdType nodeID);

  // functions for packaging and writing intermediate results
  virtual void
      CreatePackageFilenames(vtkMRMLScene* scene, const char* packageDirectoryName);
  virtual bool CreatePackageDirectories(const char* packageDirectoryName);
  virtual bool WritePackagedScene(vtkMRMLScene* scene);

  //BTX
  void
      AddDefaultTasksToList(const char* FilePath, std::vector<std::string> & DefaultTasksName, std::vector<
          std::string> & DefaultTasksFile, std::vector<std::string> & DefinePreprocessingTasksName, std::vector<
          std::string> & DefinePreprocessingTasksFile);
  //ETX

  int StartSegmentationWithoutPreprocessingAndSaving();
  virtual int
      StartSegmentationWithoutPreprocessing(vtkSlicerApplicationLogic *appLogic);

  //
  // actions
  //
  virtual bool SaveIntermediateResults(vtkSlicerApplicationLogic *appLogic);

  // copy all nodes relating to the EMSegmenter into newScene
  // and write to file
  virtual bool
      PackageAndWriteData(vtkSlicerApplicationLogic *appLogic, const char* packageDirectoryName);

  //BTX
  std::string GetTemporaryTaskDirectory();
  //ETX

  int UpdateTasks();

  //
  // SLICER COMMON INTERFACE STARTS HERE
  //

  vtkSlicerCommonInterface* GetSlicerCommonInterface();
  virtual int SourceTclFile(const char *tclFile);

  const char* GetPluginsDirectory();

  //
  // SLICER COMMON INTERFACE ENDS HERE
  //
  virtual int SourceTaskFiles();

  virtual int SourcePreprocessingTclFiles();
  int ComputeIntensityDistributionsFromSpatialPrior();

  const char* DefineTclTaskFileFromMRML();
  const char* DefineTclFullPathName(const char* TclFileName);


  //BTX
  std::string GetTclTaskDirectory();
  void CreateDefaultTasksList(std::vector<std::string> & DefaultTasksName, 
                              std::vector<std::string> & DefaultTasksFile, 
                              std::vector<std::string> & DefinePreprocessingTasksName, 
                              std::vector<std::string> & DefinePreprocessingTasksFile);
  //ETX

  void UpdateIntensityDistributionAuto(vtkIdType nodeID);

#ifdef Slicer3_USE_KWWIDGETS  
  void RunAtlasCreator(vtkMRMLAtlasCreatorNode *node);
#endif

  void WriteImage(vtkImageData* file, const char* filename);


#ifdef Slicer3_USE_KWWIDGETS
  // we do not want to wrap this function in Slicer3
  // it is only used in Slicer4

  //BTX
#endif
std::string GetTasks();
std::string GetPreprocessingTasks();

#ifdef Slicer3_USE_KWWIDGETS
  //ETX
#endif

  int GetSlicerVersion();

  // Remove all temporary files and directories generated by the logic 
  void RemoveTaskAndTempFiles();
  void RemoveTempFilesAndDirs();
 
protected:

  //BTX
  template<class T>
  static T GuessRegistrationBackgroundLevel(vtkImageData* imageData);
  //ETX

  static void
      ComposeGridTransform(vtkGridTransform* inGrid, vtkMatrix4x4* preMultiply, vtkMatrix4x4* postMultiply, vtkGridTransform* outGrid);

  // Just provide tcl file name and correct directory will be added 
  virtual int SourceFileInTaskDirectory(const char *tclFile);

  // Description:
  // Convenience method for determining if two volumes have same geometry
  static bool
      IsVolumeGeometryEqual(vtkMRMLVolumeNode* lhs, vtkMRMLVolumeNode* rhs);

  static void PrintImageInfo(vtkMRMLVolumeNode* volumeNode);
  static void PrintImageInfo(vtkImageData* image);

  // copy data from MRML to algorithm
  virtual void CopyAtlasDataToSegmenter(vtkImageEMLocalSegmenter* segmenter);
  virtual void CopyTargetDataToSegmenter(vtkImageEMLocalSegmenter* segmenter);
  virtual void CopyGlobalDataToSegmenter(vtkImageEMLocalSegmenter* segmenter);
  virtual void
      CopyTreeDataToSegmenter(vtkImageEMLocalSuperClass* node, vtkIdType nodeID);
  virtual void
      CopyTreeGenericDataToSegmenter(vtkImageEMLocalGenericClass* node, vtkIdType nodeID);
  virtual void
      CopyTreeParentDataToSegmenter(vtkImageEMLocalSuperClass* node, vtkIdType nodeID);
  virtual void
      CopyTreeLeafDataToSegmenter(vtkImageEMLocalClass* node, vtkIdType nodeID);

  //
  // convenience methods for translating enums between algorithm and
  // this module
  virtual int
  ConvertGUIEnumToAlgorithmEnumStoppingConditionType(int guiEnumValue);
  virtual int
  ConvertGUIEnumToAlgorithmEnumInterpolationType(int guiEnumValue);

  // not currently used
  vtkSetStringMacro(ProgressCurrentAction)
  ;
  vtkSetMacro(ProgressGlobalFractionCompleted, double)
  ;
  vtkSetMacro(ProgressCurrentFractionCompleted, double)
  ;

  const char* GetTemporaryDirectory();

  bool CreateIntermediateDirectory(); 

  // 
  int
      ActiveMeanField(vtkImageEMLocalSegmenter* segmenter, vtkImageData* result);
  void
      InitializeLevelSet(vtkImageLevelSets* levelset, vtkImageData* initVolume);

  //
  // because the mrml nodes are very complicated for this module, we
  // delegate the handeling of them to a MRML manager
  vtkEMSegmentMRMLManager* MRMLManager;

  char *ModuleName;
  char *CurrentTmpFileName;

  //
  // information related to progress bars: this mechanism is not
  // currently implemented and might me best implemented elsewhere
  char* ProgressCurrentAction;
  double ProgressGlobalFractionCompleted;
  double ProgressCurrentFractionCompleted;
  //BTX
  std::string ErrorMsg;
  //ETX
  vtkEMSegmentLogic();
  ~vtkEMSegmentLogic();

private:
  vtkEMSegmentLogic(const vtkEMSegmentLogic&);
  void operator=(const vtkEMSegmentLogic&);

  vtkSlicerCommonInterface *SlicerCommonInterface;

  //BTX
  std::string StringHolder;

  std::vector<std::string> tempFileList; 
  std::vector<std::string> tempDirList; 
  //ETX



};

#endif
