DO $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM information_schema.tables
        WHERE table_schema = 'public'
          AND table_name = 'object_geometry'
    ) THEN
        IF NOT EXISTS (
            SELECT 1
            FROM pg_constraint
            WHERE conname = 'chk_object_geometry_type'
        ) THEN
            ALTER TABLE public.object_geometry
                ADD CONSTRAINT chk_object_geometry_type
                CHECK (geometry_type IN ('Point', 'LineString', 'Polygon'));
        END IF;

        IF NOT EXISTS (
            SELECT 1
            FROM pg_constraint
            WHERE conname = 'chk_object_geometry_point_order'
        ) THEN
            ALTER TABLE public.object_geometry
                ADD CONSTRAINT chk_object_geometry_point_order
                CHECK (point_order >= 0);
        END IF;

        IF NOT EXISTS (
            SELECT 1
            FROM pg_constraint
            WHERE conname = 'chk_object_geometry_role_type'
        ) THEN
            ALTER TABLE public.object_geometry
                ADD CONSTRAINT chk_object_geometry_role_type
                CHECK (
                    lower(geometry_role) NOT IN ('position', 'route', 'coverage')
                    OR (lower(geometry_role) = 'position' AND geometry_type = 'Point')
                    OR (lower(geometry_role) = 'route' AND geometry_type = 'LineString')
                    OR (lower(geometry_role) = 'coverage' AND geometry_type = 'Polygon')
                );
        END IF;
    END IF;
END
$$;
