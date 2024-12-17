; ModuleID = 'Module'
source_filename = "Module"

declare void @simFlush()

declare void @simInit()

declare void @simExit()

declare void @simClearWindow()

declare void @simPutPixel(i32, i32, i32)

define double @main() {
entry:
  call void @simInit()
  %step = alloca double, align 8
  store double 0.000000e+00, ptr %step, align 8
  %y = alloca double, align 8
  store double 0.000000e+00, ptr %y, align 8
  %x = alloca double, align 8
  store double 0.000000e+00, ptr %x, align 8
  br label %while_test1

while_test1:                                      ; preds = %while_end6, %entry
  %0 = load double, ptr %step, align 8
  %1 = fcmp ult double %0, 1.000000e+03
  br i1 %1, label %while_loop2, label %while_end3

while_loop2:                                      ; preds = %while_test1
  br label %while_test4

while_end3:                                       ; preds = %while_test1
  call void @simExit()
  ret double 0.000000e+00

while_test4:                                      ; preds = %while_end9, %while_loop2
  %2 = load double, ptr %y, align 8
  %3 = fcmp ult double %2, 1.920000e+03
  br i1 %3, label %while_loop5, label %while_end6

while_loop5:                                      ; preds = %while_test4
  br label %while_test7

while_end6:                                       ; preds = %while_test4
  call void @simFlush()
  br label %while_test1

while_test7:                                      ; preds = %while_loop8, %while_loop5
  %4 = load double, ptr %x, align 8
  %5 = fcmp ult double %4, 1.080000e+03
  br i1 %5, label %while_loop8, label %while_end9

while_loop8:                                      ; preds = %while_test7
  %6 = load double, ptr %x, align 8
  %7 = load double, ptr %y, align 8
  %8 = fmul double %6, %7
  %9 = load double, ptr %step, align 8
  %10 = fmul double %8, %9
  %11 = fadd double 0x41EFE00000000000, %10
  %12 = fptosi double %11 to i32
  %13 = load double, ptr %y, align 8
  %14 = fptosi double %13 to i32
  %15 = load double, ptr %x, align 8
  %16 = fptosi double %15 to i32
  call void @simPutPixel(i32 %16, i32 %14, i32 %12)
  br label %while_test7

while_end9:                                       ; preds = %while_test7
  br label %while_test4
}
