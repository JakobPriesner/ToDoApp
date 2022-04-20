import { AfterViewInit, Component, ElementRef, EventEmitter, Input, Output, ViewChild } from "@angular/core";
import { List } from "src/lib/data-access/models";
import {BackendService} from '../../data-access/service/backend.service';

@Component({
    selector: 'listObject',
    templateUrl: './listObject.component.html',
    styleUrls: ['./listObject.component.scss'],
})
export class ListObjectComponent implements AfterViewInit{
    @Input() list: List;
    @Input() onCreate: boolean;
    @Output() clickDeleteEvent = new EventEmitter<number>();
    @Output() listSelectedEvent = new EventEmitter<List>();

    @ViewChild('myInput') myInput: ElementRef;

    showTrash: boolean = false;
    elementRef: ElementRef;

    constructor( private backendService: BackendService ) { }

    ngAfterViewInit(): void {
        if (this.onCreate) {
            this.myInput.nativeElement.focus();
            this.onSelectEvent();
        }
    }

    onMouseEnter(): void {
        this.showTrash = true;
    }

    onMouseLeave(): void {
        this.showTrash = false;
    }

    onDeleteEvent(): void {
        this.clickDeleteEvent.emit(this.list.id);
    }

    onSelectEvent(): void {
        this.list.selected = true;
        this.listSelectedEvent.emit(this.list);
    }

    onTitleChange(event: any): void {
        if (event.target.value != "") this.list.title = event.target.value;
        if (this.list.title == "") this.list.title = "New List";
        this.backendService.putList(this.list).subscribe();
    }
}
