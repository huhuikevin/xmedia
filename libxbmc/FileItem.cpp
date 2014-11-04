/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#include "URL.h"
#include "FileItem.h"

CFileItem::CFileItem(const CFileItem& item)
{
  m_musicInfoTag = NULL;
  m_videoInfoTag = NULL;
  m_epgInfoTag = NULL;
  m_pvrChannelInfoTag = NULL;
  m_pvrRecordingInfoTag = NULL;
  m_pvrTimerInfoTag = NULL;
  m_pictureInfoTag = NULL;
  *this = item;
}

CFileItem::CFileItem(const CGUIListItem& item)
{
  Initialize();
  // not particularly pretty, but it gets around the issue of Initialize() defaulting
  // parameters in the CGUIListItem base class.


  FillInMimeType(false);
}

CFileItem::CFileItem(void)
{
  Initialize();
}

CFileItem::CFileItem(const std::string& strLabel)
{
  Initialize();
  SetLabel(strLabel);
}

CFileItem::CFileItem(const CURL& path, bool bIsFolder)
{
  Initialize();
  m_strPath = path.Get();
  m_bIsFolder = bIsFolder;
  // tuxbox urls cannot have a / at end
  if (m_bIsFolder && !m_strPath.empty() && !IsFileFolder() && !URIUtils::IsTuxBox(m_strPath))
    URIUtils::AddSlashAtEnd(m_strPath);
  FillInMimeType(false);
}

CFileItem::CFileItem(const std::string& strPath, bool bIsFolder)
{
  Initialize();
  m_strPath = strPath;
  m_bIsFolder = bIsFolder;
  // tuxbox urls cannot have a / at end
  if (m_bIsFolder && !m_strPath.empty() && !IsFileFolder() && !URIUtils::IsTuxBox(m_strPath))
    URIUtils::AddSlashAtEnd(m_strPath);
  FillInMimeType(false);
}



CFileItem::~CFileItem(void)
{

}

const CFileItem& CFileItem::operator=(const CFileItem& item)
{
  if (this == &item) return * this;

  m_bLabelPreformated=item.m_bLabelPreformated;
  FreeMemory();
  m_strPath = item.GetPath();
  m_bIsParentFolder = item.m_bIsParentFolder;
  m_iDriveType = item.m_iDriveType;
  m_bIsShareOrDrive = item.m_bIsShareOrDrive;
  m_dateTime = item.m_dateTime;
  m_dwSize = item.m_dwSize;

  m_lStartOffset = item.m_lStartOffset;
  m_lStartPartNumber = item.m_lStartPartNumber;
  m_lEndOffset = item.m_lEndOffset;
  m_strDVDLabel = item.m_strDVDLabel;
  m_strTitle = item.m_strTitle;
  m_iprogramCount = item.m_iprogramCount;
  m_idepth = item.m_idepth;
  m_iLockMode = item.m_iLockMode;
  m_strLockCode = item.m_strLockCode;
  m_iHasLock = item.m_iHasLock;
  m_iBadPwdCount = item.m_iBadPwdCount;
  m_bCanQueue=item.m_bCanQueue;
  m_mimetype = item.m_mimetype;
  m_extrainfo = item.m_extrainfo;
  m_specialSort = item.m_specialSort;
  m_bIsAlbum = item.m_bIsAlbum;
  return *this;
}

void CFileItem::Initialize()
{

  m_bLabelPreformated=false;
  m_bIsAlbum = false;
  m_dwSize = 0;
  m_bIsParentFolder=false;
  m_bIsShareOrDrive = false;
  m_lStartOffset = 0;
  m_lStartPartNumber = 1;
  m_lEndOffset = 0;
  m_iprogramCount = 0;
  m_idepth = 1;
  m_iLockMode = LOCK_MODE_EVERYONE;
  m_iBadPwdCount = 0;
  m_iHasLock = 0;
  m_bCanQueue=true;
}

void CFileItem::Reset()
{
  // CGUIListItem members...
  m_strLabel2.clear();
  SetLabel("");
//  FreeIcons();
//  m_overlayIcon = ICON_OVERLAY_NONE;
  m_bSelected = false;
  m_bIsFolder = false;

  m_strDVDLabel.clear();
  m_strTitle.clear();
  m_strPath.clear();
  m_dateTime.Reset();
  m_strLockCode.clear();
  m_mimetype.clear();

  m_extrainfo.clear();
  ClearProperties();

  Initialize();
  SetInvalid();
}



bool CFileItem::Exists(bool bUseCache /* = true */) const
{
  std::string strPath = m_strPath;

  if (m_bIsFolder)
    return CDirectory::Exists(strPath, bUseCache);
  else
    return CFile::Exists(strPath, bUseCache);

  return false;
}



void CFileItem::SetLabel(const std::string &strLabel)
{
  if (strLabel=="..")
  {
    m_bIsParentFolder=true;
    m_bIsFolder=true;
    SetLabelPreformated(true);
  }

}

void CFileItem::SetFileSizeLabel()
{
  if( m_bIsFolder && m_dwSize == 0 )
    SetLabel2("");
  else
    SetLabel2(StringUtils::SizeToString(m_dwSize));
}

bool CFileItem::CanQueue() const
{
  return m_bCanQueue;
}

void CFileItem::SetCanQueue(bool bYesNo)
{
  m_bCanQueue=bYesNo;
}

bool CFileItem::IsParentFolder() const
{
  return m_bIsParentFolder;
}

void CFileItem::FillInMimeType(bool lookup /*= true*/)
{

}

bool CFileItem::IsSamePath(const CFileItem *item) const
{
  if (!item)
    return false;

  if (item->GetPath() == m_strPath)
  {
    return true;
  }

  return false;
}



void CFileItem::UpdateInfo(const CFileItem &item, bool replaceLabels /*=true*/)
{

}


/*
 TODO: Ideally this (and SetPath) would not be available outside of construction
 for CFileItem objects, or at least restricted to essentially be equivalent
 to construction. This would require re-formulating a bunch of CFileItem
 construction, and also allowing CFileItemList to have it's own (public)
 SetURL() function, so for now we give direct access.
 */
void CFileItem::SetURL(const CURL& url)
{
  m_strPath = url.Get();
}

const CURL CFileItem::GetURL() const
{
  CURL url(m_strPath);
  return url;
}

bool CFileItem::IsURL(const CURL& url) const
{
  return IsPath(url.Get());
}

bool CFileItem::IsPath(const std::string& path) const
{
  return URIUtils::PathEquals(m_strPath, path);
}

/////////////////////////////////////////////////////////////////////////////////
/////
///// CFileItemList
/////
//////////////////////////////////////////////////////////////////////////////////

CFileItemList::CFileItemList()
{
  m_fastLookup = false;
  m_bIsFolder = true;
  m_cacheToDisc = CACHE_IF_SLOW;
  m_sortIgnoreFolders = false;
  m_replaceListing = false;
}

CFileItemList::CFileItemList(const std::string& strPath) : CFileItem(strPath, true)
{
  m_fastLookup = false;
  m_cacheToDisc = CACHE_IF_SLOW;
  m_sortIgnoreFolders = false;
  m_replaceListing = false;
}

CFileItemList::~CFileItemList()
{
  Clear();
}

CFileItemPtr CFileItemList::operator[] (int iItem)
{
  return Get(iItem);
}

const CFileItemPtr CFileItemList::operator[] (int iItem) const
{
  return Get(iItem);
}

CFileItemPtr CFileItemList::operator[] (const std::string& strPath)
{
  return Get(strPath);
}

const CFileItemPtr CFileItemList::operator[] (const std::string& strPath) const
{
  return Get(strPath);
}

void CFileItemList::SetFastLookup(bool fastLookup)
{
  CSingleLock lock(m_lock);

  if (fastLookup && !m_fastLookup)
  { // generate the map
    m_map.clear();
    for (unsigned int i=0; i < m_items.size(); i++)
    {
      CFileItemPtr pItem = m_items[i];
      m_map.insert(MAPFILEITEMSPAIR(pItem->GetPath(), pItem));
    }
  }
  if (!fastLookup && m_fastLookup)
    m_map.clear();
  m_fastLookup = fastLookup;
}

bool CFileItemList::Contains(const std::string& fileName) const
{
  CSingleLock lock(m_lock);

  if (m_fastLookup)
    return m_map.find(fileName) != m_map.end();

  // slow method...
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    const CFileItemPtr pItem = m_items[i];
    if (pItem->IsPath(fileName))
      return true;
  }
  return false;
}

void CFileItemList::Clear()
{
  CSingleLock lock(m_lock);

  ClearItems();


  m_replaceListing = false;
  m_content.clear();
}

void CFileItemList::ClearItems()
{
  CSingleLock lock(m_lock);
  // make sure we free the memory of the items (these are GUIControls which may have allocated resources)
  FreeMemory();
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    CFileItemPtr item = m_items[i];
    item->FreeMemory();
  }
  m_items.clear();
  m_map.clear();
}

void CFileItemList::Add(const CFileItemPtr &pItem)
{
  CSingleLock lock(m_lock);

  m_items.push_back(pItem);
  if (m_fastLookup)
  {
    m_map.insert(MAPFILEITEMSPAIR(pItem->GetPath(), pItem));
  }
}

void CFileItemList::AddFront(const CFileItemPtr &pItem, int itemPosition)
{
  CSingleLock lock(m_lock);

  if (itemPosition >= 0)
  {
    m_items.insert(m_items.begin()+itemPosition, pItem);
  }
  else
  {
    m_items.insert(m_items.begin()+(m_items.size()+itemPosition), pItem);
  }
  if (m_fastLookup)
  {
    m_map.insert(MAPFILEITEMSPAIR(pItem->GetPath(), pItem));
  }
}

void CFileItemList::Remove(CFileItem* pItem)
{
  CSingleLock lock(m_lock);

  for (IVECFILEITEMS it = m_items.begin(); it != m_items.end(); ++it)
  {
    if (pItem == it->get())
    {
      m_items.erase(it);
      if (m_fastLookup)
      {
        m_map.erase(pItem->GetPath());
      }
      break;
    }
  }
}

void CFileItemList::Remove(int iItem)
{
  CSingleLock lock(m_lock);

  if (iItem >= 0 && iItem < (int)Size())
  {
    CFileItemPtr pItem = *(m_items.begin() + iItem);
    if (m_fastLookup)
    {
      m_map.erase(pItem->GetPath());
    }
    m_items.erase(m_items.begin() + iItem);
  }
}

void CFileItemList::Append(const CFileItemList& itemlist)
{
  CSingleLock lock(m_lock);

  for (int i = 0; i < itemlist.Size(); ++i)
    Add(itemlist[i]);
}

void CFileItemList::Assign(const CFileItemList& itemlist, bool append)
{
  CSingleLock lock(m_lock);
  if (!append)
    Clear();
  Append(itemlist);
  SetPath(itemlist.GetPath());
  SetLabel(itemlist.GetLabel());

  m_content = itemlist.m_content;
  m_mapProperties = itemlist.m_mapProperties;
  m_cacheToDisc = itemlist.m_cacheToDisc;
}

bool CFileItemList::Copy(const CFileItemList& items, bool copyItems /* = true */)
{
  // assign all CFileItem parts
  *(CFileItem*)this = *(CFileItem*)&items;

  // assign the rest of the CFileItemList properties
  m_replaceListing  = items.m_replaceListing;
  m_content         = items.m_content;
  m_mapProperties   = items.m_mapProperties;


  if (copyItems)
  {
    // make a copy of each item
    for (int i = 0; i < items.Size(); i++)
    {
      CFileItemPtr newItem(new CFileItem(*items[i]));
      Add(newItem);
    }
  }

  return true;
}

CFileItemPtr CFileItemList::Get(int iItem)
{
  CSingleLock lock(m_lock);

  if (iItem > -1 && iItem < (int)m_items.size())
    return m_items[iItem];

  return CFileItemPtr();
}

const CFileItemPtr CFileItemList::Get(int iItem) const
{
  CSingleLock lock(m_lock);

  if (iItem > -1 && iItem < (int)m_items.size())
    return m_items[iItem];

  return CFileItemPtr();
}

CFileItemPtr CFileItemList::Get(const std::string& strPath)
{
  CSingleLock lock(m_lock);

  if (m_fastLookup)
  {
    IMAPFILEITEMS it=m_map.find(strPath);
    if (it != m_map.end())
      return it->second;

    return CFileItemPtr();
  }
  // slow method...
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    CFileItemPtr pItem = m_items[i];
    if (pItem->IsPath(strPath))
      return pItem;
  }

  return CFileItemPtr();
}

const CFileItemPtr CFileItemList::Get(const std::string& strPath) const
{
  CSingleLock lock(m_lock);

  if (m_fastLookup)
  {
    map<std::string, CFileItemPtr>::const_iterator it=m_map.find(strPath);
    if (it != m_map.end())
      return it->second;

    return CFileItemPtr();
  }
  // slow method...
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    CFileItemPtr pItem = m_items[i];
    if (pItem->IsPath(strPath))
      return pItem;
  }

  return CFileItemPtr();
}

int CFileItemList::Size() const
{
  CSingleLock lock(m_lock);
  return (int)m_items.size();
}

bool CFileItemList::IsEmpty() const
{
  CSingleLock lock(m_lock);
  return (m_items.size() <= 0);
}

void CFileItemList::Reserve(int iCount)
{
  CSingleLock lock(m_lock);
  m_items.reserve(iCount);
}


int CFileItemList::GetFolderCount() const
{
  CSingleLock lock(m_lock);
  int nFolderCount = 0;
  for (int i = 0; i < (int)m_items.size(); i++)
  {
    CFileItemPtr pItem = m_items[i];
    if (pItem->m_bIsFolder)
      nFolderCount++;
  }

  return nFolderCount;
}

int CFileItemList::GetObjectCount() const
{
  CSingleLock lock(m_lock);

  int numObjects = (int)m_items.size();
  if (numObjects && m_items[0]->IsParentFolder())
    numObjects--;

  return numObjects;
}

int CFileItemList::GetFileCount() const
{
  CSingleLock lock(m_lock);
  int nFileCount = 0;
  for (int i = 0; i < (int)m_items.size(); i++)
  {
    CFileItemPtr pItem = m_items[i];
    if (!pItem->m_bIsFolder)
      nFileCount++;
  }

  return nFileCount;
}



// Remove the extensions from the filenames
void CFileItemList::RemoveExtensions()
{
  CSingleLock lock(m_lock);
  for (int i = 0; i < Size(); ++i)
    m_items[i]->RemoveExtension();
}



void CFileItemList::Swap(unsigned int item1, unsigned int item2)
{
  if (item1 != item2 && item1 < m_items.size() && item2 < m_items.size())
    std::swap(m_items[item1], m_items[item2]);
}

bool CFileItemList::UpdateItem(const CFileItem *item)
{
  if (!item) return false;

  CSingleLock lock(m_lock);
  for (unsigned int i = 0; i < m_items.size(); i++)
  {
    CFileItemPtr pItem = m_items[i];
    if (pItem->IsSamePath(item))
    {
      pItem->UpdateInfo(*item);
      return true;
    }
  }
  return false;
}


void CFileItemList::SetReplaceListing(bool replace)
{
  m_replaceListing = replace;
}


