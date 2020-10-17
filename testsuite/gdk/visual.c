#include <gdk/gdk.h>

/* We don't technically guarantee that the visual returned by
 * gdk_screen_get_rgba_visual is ARGB8888. But if it isn't, lots
 * of code will break, so test this here anyway.
 * The main point of this test is to ensure that the pixel_details
 * functions return meaningful values for TrueColor visuals.
 */
static void
test_rgba_visual (void)
{
  GdkScreen *screen;
  GdkVisual *visual;
  guint32 r_mask, g_mask, b_mask;
  gint r_shift, g_shift, b_shift;
  gint r_precision, g_precision, b_precision;
  gint depth;
  GdkVisualType type;

  g_test_bug ("764210");

  screen = gdk_screen_get_default ();
  visual = gdk_screen_get_rgba_visual (screen);

  if (visual == NULL)
    {
      g_test_skip ("no rgba visual");
      return;
    }

  depth = gdk_visual_get_depth (visual);
  type = gdk_visual_get_visual_type (visual);
  gdk_visual_get_red_pixel_details (visual, &r_mask, &r_shift, &r_precision);
  gdk_visual_get_green_pixel_details (visual, &g_mask, &g_shift, &g_precision);
  gdk_visual_get_blue_pixel_details (visual, &b_mask, &b_shift, &b_precision);

  g_assert_cmpint (depth, ==, 32);
  g_assert_cmpint (type, ==, GDK_VISUAL_TRUE_COLOR);

  g_assert_cmphex (r_mask, ==, 0x00ff0000);
  g_assert_cmphex (g_mask, ==, 0x0000ff00);
  g_assert_cmphex (b_mask, ==, 0x000000ff);

  g_assert_cmpint (r_shift, ==, 16);
  g_assert_cmpint (g_shift, ==,  8);
  g_assert_cmpint (b_shift, ==,  0);

  g_assert_cmpint (r_precision, ==, 8);
  g_assert_cmpint (g_precision, ==, 8);
  g_assert_cmpint (b_precision, ==, 8);
}

int
main (int argc, char *argv[])
{
        g_test_init (&argc, &argv, NULL);

        gdk_init (NULL, NULL);

        g_test_bug_base ("http://bugzilla.gnome.org/");

        g_test_add_func ("/visual/rgba", test_rgba_visual);

        return g_test_run ();
}
