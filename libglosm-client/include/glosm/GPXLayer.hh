/*
 * Copyright (C) 2010-2011 Dmitry Marakasov
 *
 * This file is part of glosm.
 *
 * glosm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glosm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glosm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPXLAYER_HH
#define GPXLAYER_HH

#include <glosm/Layer.hh>
#include <glosm/Projection.hh>
#include <glosm/NonCopyable.hh>
#include <glosm/TileManager.hh>

class Viewer;
class GPXDatasource;

/**
 * Layer with 3D OpenStreetMap data.
 */
class GPXLayer : public Layer, public TileManager, private NonCopyable {
protected:
	const Projection projection_;
	const GPXDatasource& datasource_;

public:
	GPXLayer(const Projection projection, const GPXDatasource& datasource);
	virtual ~GPXLayer();

	void Render(const Viewer& viewer);
	virtual Tile* SpawnTile(const BBoxi& bbox, int flags) const;
};

#endif
