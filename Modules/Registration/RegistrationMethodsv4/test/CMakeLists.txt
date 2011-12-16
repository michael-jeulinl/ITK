itk_module_test()
set(ITKRegistrationMethodsv4Tests
itkSimpleImageRegistrationTest.cxx
itkTimeVaryingVelocityFieldImageRegistrationTest.cxx
itkQuasiNewtonOptimizerv4RegistrationTest.cxx
)

set(INPUTDATA ${ITK_DATA_ROOT}/Input)
set(BASELINE_ROOT ${ITK_DATA_ROOT}/Baseline)
set(TEMP ${ITK_TEST_OUTPUT_DIR})

CreateTestDriver(ITKRegistrationMethodsv4  "${ITKRegistrationMethodsv4-Test_LIBRARIES}" "${ITKRegistrationMethodsv4Tests}")

itk_add_test(NAME itkSimpleImageRegistrationTest
      COMMAND ITKRegistrationMethodsv4TestDriver
      --with-threads 2
      --compare DATA{Baseline/itkSimpleImageRegistrationTest.nii.gz}
                ${TEMP}/itkSimpleImageRegistrationTest.nii.gz
              itkSimpleImageRegistrationTest
              2 # number of dimensions
              DATA{Input/r16slice_rigid.nii.gz}
              DATA{Input/r64slice.nii.gz}
              ${TEMP}/itkSimpleImageRegistrationTest.nii.gz
              100 # number of affine iterations
              10 # number of deformable iterations
              )

itk_add_test(NAME itkTimeVaryingVelocityFieldImageRegistrationTest
      COMMAND ITKRegistrationMethodsv4TestDriver
              itkTimeVaryingVelocityFieldImageRegistrationTest
              2 # number of dimensions
              DATA{Input/r16slice_rigid.nii.gz}
              DATA{Input/r64slice.nii.gz}
              ${TEMP}/itkTimeVaryingVelocityFieldImageRegistrationTest
              10  # number of optimization iterations of the displacement field
              0.5 # learning rate
              )

itk_add_test(NAME itkQuasiNewtonOptimizerv4RegistrationTest1
      COMMAND ITKRegistrationMethodsv4TestDriver
              itkQuasiNewtonOptimizerv4RegistrationTest
              2 # number of dimensions
              dm # Demons metric
              DATA{Input/r16slice_rigid.nii.gz}
              DATA{Input/r64slice.nii.gz}
              ${TEMP}/itkQuasiNewtonOptimizerv4RegistrationTest1.nii.gz
              5 2 )

itk_add_test(NAME itkQuasiNewtonOptimizerv4RegistrationTest2
      COMMAND ITKRegistrationMethodsv4TestDriver
              itkQuasiNewtonOptimizerv4RegistrationTest
              2 # number of dimensions
              mi # metric of joint histogram mutual information
              DATA{Input/r16slice_rigid.nii.gz}
              DATA{Input/r64slice.nii.gz}
              ${TEMP}/itkQuasiNewtonOptimizerv4RegistrationTest2.nii.gz
              5 2 )

itk_add_test(NAME itkQuasiNewtonOptimizerv4RegistrationTest3
      COMMAND ITKRegistrationMethodsv4TestDriver
              itkQuasiNewtonOptimizerv4RegistrationTest
              2 # number of dimensions
              anc # metric of ants neighborhood correlation
              DATA{Input/r16slice_rigid.nii.gz}
              DATA{Input/r64slice.nii.gz}
              ${TEMP}/itkQuasiNewtonOptimizerv4RegistrationTest3.nii.gz
              5 2 )